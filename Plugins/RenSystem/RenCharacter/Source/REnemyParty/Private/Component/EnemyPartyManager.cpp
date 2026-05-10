// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Component/EnemyPartyManager.h"

// Engine Headers
#include "GameFramework/Character.h"
#include "UObject/ObjectSaveContext.h"

// Project Headers
#include "Asset/CharacterAsset.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Manager/RAssetManager.inl"


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
}



void UEnemyManagerComponent::GetSpawningAssetIds(TArray<FPrimaryAssetId>& OutAssetIds) const
{
	OutAssetIds.Empty();
	for (const FEnemyInitializationData& Data : SpawnData)
	{
		OutAssetIds.AddUnique(Data.AssetId);
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

	TArray<FName> AssetBundles;

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
	for (const FEnemyInitializationData& Data : SpawnData)
	{
		SpawnEnemy(Data);
	}
}

void UEnemyManagerComponent::SpawnEnemy(const FEnemyInitializationData& Data)
{
	const UCharacterAsset* Asset = AssetManager->GetPrimaryAssetObject<UCharacterAsset>(Data.AssetId);
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

	FTransform SpawnTransform;
	ACharacterBase* Character = GetWorld()->SpawnActorDeferred<ACharacterBase>(CharacterClass, SpawnTransform, GetOwner(), nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	if (!IsValid(Character))
	{
		LOG_ERROR(LogCharacterParty, TEXT("Spawned enemy is invalid"));
		return;
	}

	Character->CharacterAsset = Asset;
	Character->CharacterData.Attributes = Data.Attributes;
	Character->CharacterData.AssetId = Data.AssetId;
	Character->CharacterData.Metadata = Data.Metadata;
	Character->InitializeCharacter();
	Character->FinishSpawning(SpawnTransform);

	RegisterEnemy(Character);
}

void UEnemyManagerComponent::RegisterEnemy(ACharacterBase* Character)
{
	if (IsValid(Character))
	{
		Character->OnCharacterRevived.AddUObject(this, &UEnemyManagerComponent::HandleOnEnemyDataChanged, Character);
		Character->OnCharacterDied.AddUObject(this, &UEnemyManagerComponent::HandleOnEnemyDataChanged, Character);

		ActiveEnemies.Add(Character);
	}
}

void UEnemyManagerComponent::UnregisterEnemy(ACharacterBase* Character)
{
	if (IsValid(Character))
	{
		Character->OnCharacterRevived.RemoveAll(this);
		Character->OnCharacterDied.RemoveAll(this);

		Character->DeinitializeCharacter();
		Character->Destroy();
	}
}

void UEnemyManagerComponent::CleanupEnemies()
{	
	for (TObjectPtr<ACharacterBase>& Enemy : ActiveEnemies)
	{
		UnregisterEnemy(Enemy.Get());
	}
	ActiveEnemies.Empty();
}

void UEnemyManagerComponent::HandleOnEnemyDataChanged(ACharacterBase* Character)
{
	PRINT_WARNING(LogCharacterParty, 2.0f, TEXT("Enemy Died"));

	CheckAllEnemies();
}



void UEnemyManagerComponent::CheckAllEnemies()
{
	int AliveCount = 0;
	for(TObjectPtr<ACharacterBase>& Enemy : ActiveEnemies)
	{
		ACharacterBase* Character = Enemy.Get();
		if (IsValid(Character) && Character->IsAlive())
		{
			AliveCount++;
		}
	}

	if (AliveCount == 0)
	{
		PRINT_WARNING(LogCharacterParty, 2.0f, TEXT("All Enemies Died"));
		OnAllEnemyDied.Broadcast();
	}
}

