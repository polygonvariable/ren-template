// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Component/EnemyManagerComponent.h"

// Engine Headers
#include "UObject/ObjectSaveContext.h"
#include "Abilities/GameplayAbilityTypes.h"

// Project Headers
#include "Actor/EnemyCharacter.h"
#include "Asset/CharacterAsset.h"
#include "Log/LogCategory.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Log/LogMacro.h"
#include "Manager/RAssetManager.inl"
#include "Settings/EnemySettings.h"
#include "Subsystem/BroadcastSubsystem.h"
#include "Definition/Runtime/EnemyPayload.h"
#include "Settings/CharacterSettings.h"


UEnemyManagerComponent::UEnemyManagerComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	bAutoActivate = false;
	SetIsReplicatedByDefault(true);
}


void UEnemyManagerComponent::BeginPlay()
{
	AssetManager = URAssetManager::Get();
	Super::BeginPlay();
}

void UEnemyManagerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	CleanupEnemies();

	AssetManager = nullptr;
	Super::EndPlay(EndPlayReason);
}

void UEnemyManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UEnemyManagerComponent::PreSave(FObjectPreSaveContext ObjectSaveContext)
{
	Super::PreSave(ObjectSaveContext);

#if WITH_EDITOR
	for (FEnemySpawnData& Data : SpawnData)
	{
		Data.EnemyData.EnemyId = FGuid::NewGuid();
	}
#endif
}

void UEnemyManagerComponent::GetSpawningAssetIds(TArray<FPrimaryAssetId>& OutAssetIds) const
{
	OutAssetIds.Empty();
	for (const FEnemySpawnData& Data : SpawnData)
	{
		OutAssetIds.AddUnique(Data.CharacterData.AssetId);
	}
}

void UEnemyManagerComponent::TrySpawnEnemies()
{
	CleanupEnemies();

	TArray<FPrimaryAssetId> AssetIds;
	GetSpawningAssetIds(AssetIds);

	if (AssetIds.Num() == 0)
	{
		return;
	}

	AssetManager->CancelFetch(_SpawnId);

	_SpawnId = FGuid::NewGuid();

	const UCharacterSettings* Settings = UCharacterSettings::Get();
	const TArray<FName>& AssetBundles = Settings->CharacterBundles;

	TWeakObjectPtr<UEnemyManagerComponent> WeakThis(this);
	TFuture<FLatentLoadedAssets<UCharacterAsset>> Future = AssetManager->FetchPrimaryAssets<UCharacterAsset>(_SpawnId, AssetIds, AssetBundles, false);
	Future.Next([WeakThis](const FLatentLoadedAssets<UCharacterAsset>& Result)
		{
			UEnemyManagerComponent* This = WeakThis.Get();
			if (IsValid(This) && Result.IsCompleted())
			{
				This->SpawnEnemies();
			}
		}
	);
}

void UEnemyManagerComponent::SpawnEnemies()
{
	for (const FEnemySpawnData& Data : SpawnData)
	{
		SpawnEnemy(Data);
	}
}

void UEnemyManagerComponent::SpawnEnemy(const FEnemySpawnData& Data)
{
	const UCharacterAsset* Asset = AssetManager->GetPrimaryAssetObject<UCharacterAsset>(Data.CharacterData.AssetId);
	if (!IsValid(Asset))
	{
		LOG_ERROR(LogCharacterParty, TEXT("Failed to get enemy asset"));
		return;
	}

	UClass* CharacterClass = Asset->CharacterClass.Get();
	if (!CharacterClass)
	{
		LOG_ERROR(LogCharacterParty, TEXT("CharacterClass is invalid"));
		return;
	}

	UWorld* World = GetWorld();
	AActor* Owner = GetOwner();

	FTransform SpawnTransform = Owner->GetActorTransform() * Data.EnemyData.SpawnTransform;
	AEnemyCharacter* Character = World->SpawnActorDeferred<AEnemyCharacter>(CharacterClass, SpawnTransform, Owner, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	if (!IsValid(Character))
	{
		LOG_ERROR(LogCharacterParty, TEXT("Spawned enemy is invalid"));
		return;
	}
	
	FGuid EnemyId = FGuid::NewGuid();

	Character->CharacterAsset = Asset;
	Character->CharacterData = Data.CharacterData;
	Character->EnemyData = Data.EnemyData;
	Character->EnemyData.EnemyId = EnemyId;
	Character->InitializeCharacter();
	Character->FinishSpawning(SpawnTransform);

	RegisterEnemy(EnemyId, Character);
}

void UEnemyManagerComponent::RegisterEnemy(FGuid EnemyId, AEnemyCharacter* Character)
{
	if (IsValid(Character))
	{
		Character->OnCharacterDied.AddUObject(this, &UEnemyManagerComponent::OnEnemyDataUpdated);
		Character->OnCharacterRevived.AddUObject(this, &UEnemyManagerComponent::OnEnemyDataUpdated);

		ActiveEnemies.Add(EnemyId, Character);
	}
}

void UEnemyManagerComponent::UnregisterEnemy(AEnemyCharacter* Character)
{
	if (IsValid(Character))
	{
		Character->OnCharacterDied.RemoveAll(this);
		Character->OnCharacterRevived.RemoveAll(this);

		Character->DeinitializeCharacter();
		Character->Destroy();
	}
}

void UEnemyManagerComponent::CleanupEnemies()
{	
	for (TPair<FGuid, TObjectPtr<AEnemyCharacter>>& Kv : ActiveEnemies)
	{
		UnregisterEnemy(Kv.Value);
	}
	ActiveEnemies.Empty();
}

void UEnemyManagerComponent::OnEnemyDataUpdated()
{
	int TotalCount = ActiveEnemies.Num();
	int RemainingCount = GetRemainingEnemiesCount();

	if (RemainingCount <= 0)
	{
		PRINT_WARNING(LogTemp, 2.0f, TEXT("All enemies died"));
	}
}

int UEnemyManagerComponent::GetRemainingEnemiesCount() const
{
	int AliveCount = 0;
	for (const TPair<FGuid, TObjectPtr<AEnemyCharacter>>& Kv : ActiveEnemies)
	{
		AEnemyCharacter* Enemy = Kv.Value.Get();
		if (IsValid(Enemy) && Enemy->IsAlive())
		{
			++AliveCount;
		}
	}
	return AliveCount;
}

