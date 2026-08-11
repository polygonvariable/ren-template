// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Component/EnemyManagerComponent.h"

// Engine Headers
#include "Engine/AssetManager.h"
#include "UObject/ObjectSaveContext.h"

// Project Headers
#include "Actor/EnemyCharacter.h"
#include "Data/CharacterAsset.h"
#include "Library/AssetManagerUtil.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Core/CharacterSettings.h"
#include "Core/EnemySettings.h"
#include "System/EnemyStorageManager.h"
#include "Subsystem/BroadcastSubsystem.h"
#include "System/EnemySubsystem.h"


UEnemyManagerComponent::UEnemyManagerComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	bAutoActivate = false;
	SetIsReplicatedByDefault(true);
}

void UEnemyManagerComponent::BeginPlay()
{
	AssetManager = UAssetManager::GetIfInitialized();

	UEnemySubsystem* EnemySubsystem = UEnemySubsystem::Get(GetWorld());
	if (IsValid(EnemySubsystem))
	{
		StorageManager = EnemySubsystem->GetStorageManager();
	}

	Super::BeginPlay();
}

void UEnemyManagerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	FAssetManagerUtil::CancelHandle(_SpawnHandle);
	CleanupEnemies();

	AssetManager = nullptr;
	StorageManager = nullptr;

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
		FGuid& EnemyId = Data.EnemyData.EnemyId;
		if (!EnemyId.IsValid())
		{
			EnemyId = FGuid::NewGuid();
		}
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

	FAssetManagerUtil::CancelHandle(_SpawnHandle);

	const UCharacterSettings* Settings = UCharacterSettings::Get();
	const TArray<FName>& AssetBundles = Settings->CharacterBundles;

	_SpawnHandle = AssetManager->LoadPrimaryAssets(AssetIds, AssetBundles, FStreamableDelegate::CreateUObject(this, &UEnemyManagerComponent::SpawnEnemies));
}

void UEnemyManagerComponent::SpawnEnemies()
{
	FAssetManagerUtil::ReleaseHandle(_SpawnHandle);

	for (const FEnemySpawnData& Data : SpawnData)
	{
		SpawnEnemy(Data);
	}
}

void UEnemyManagerComponent::SpawnEnemy(const FEnemySpawnData& Data)
{
	const FCharacterInitializationData& CharacterData = Data.CharacterData;
	const FEnemyInitializationData& EnemyData = Data.EnemyData;

	if (!SpawnCondition(Data))
	{
		LOG_WARNING(LogCharacterParty, TEXT("Spawn condition failed"));
		return;
	}

	const UCharacterAsset* Asset = AssetManager->GetPrimaryAssetObject<UCharacterAsset>(CharacterData.AssetId);
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

	FTransform SpawnTransform = Owner->GetActorTransform() * EnemyData.SpawnTransform;
	AEnemyCharacter* Character = World->SpawnActorDeferred<AEnemyCharacter>(CharacterClass, SpawnTransform, Owner, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	if (!IsValid(Character))
	{
		LOG_ERROR(LogCharacterParty, TEXT("Spawned enemy is invalid"));
		return;
	}
	
	Character->CharacterAsset = Asset;
	Character->CharacterData = CharacterData;
	Character->EnemyData = EnemyData;
	Character->InitializeCharacter();
	Character->FinishSpawning(SpawnTransform);

	RegisterEnemy(EnemyData.EnemyId, Character);
}

bool UEnemyManagerComponent::SpawnCondition(const FEnemySpawnData& Data) const
{
	const FEnemyInitializationData& EnemyData = Data.EnemyData;

	if (EnemyData.RespawnType == EEnemyRespawnType::SpawnOnce)
	{
		if (IsValid(StorageManager) && StorageManager->ContainsKillTimestamp(EnemyData.EnemyId))
		{
			return false;
		}
	}
	else if (EnemyData.RespawnType == EEnemyRespawnType::RespawnAfterDelay)
	{
		if (IsValid(StorageManager))
		{
			FDateTime KillTimestamp = StorageManager->GetKillTimestamp(EnemyData.EnemyId);
			FDateTime ExpectedRespawnTime = KillTimestamp + EnemyData.RespawnDelay;
			FDateTime Now = FDateTime::Now();

			if (ExpectedRespawnTime > Now)
			{
				return false;
			}
		}
	}
	else if (EnemyData.RespawnType == EEnemyRespawnType::RespawnImmediately)
	{
		return true;
	}
	else if (EnemyData.RespawnType == EEnemyRespawnType::Never)
	{
		return false;
	}

	return true;
}

void UEnemyManagerComponent::RegisterEnemy(FGuid EnemyId, AEnemyCharacter* Character)
{
	if (IsValid(Character))
	{
		Character->OnCharacterDied.AddUObject(this, &UEnemyManagerComponent::HandleOnEnemyStateChanged, EnemyId, false);
		Character->OnCharacterRevived.AddUObject(this, &UEnemyManagerComponent::HandleOnEnemyStateChanged, EnemyId, true);

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

void UEnemyManagerComponent::HandleOnEnemyStateChanged(FGuid EnemyId, bool bIsAlive)
{
	const FEnemySpawnData* Data = SpawnData.FindByPredicate([EnemyId](const FEnemySpawnData& Data) { return Data.EnemyData.EnemyId == EnemyId; });
	if (Data)
	{
		HandleEnemyTimestamp(Data, bIsAlive);
		HandleEnemyDrop(Data, bIsAlive);
	}
}

void UEnemyManagerComponent::HandleEnemyTimestamp(const FEnemySpawnData* Data, bool bIsAlive)
{
	const FEnemyInitializationData& EnemyData = Data->EnemyData;
	const EEnemyRespawnType& RespawnType = EnemyData.RespawnType;

	if (IsValid(StorageManager) && (RespawnType == EEnemyRespawnType::SpawnOnce || RespawnType == EEnemyRespawnType::RespawnAfterDelay))
	{
		if (bIsAlive)
		{
			StorageManager->RemoveKillTimestamp(EnemyData.EnemyId);
		}
		else
		{
			StorageManager->AddKillTimestamp(EnemyData.EnemyId);
		}
	}
}

void UEnemyManagerComponent::HandleEnemyDrop(const FEnemySpawnData* Data, bool bIsAlive)
{
	const FEnemyInitializationData& EnemyData = Data->EnemyData;
	if (EnemyData.bEnableDrop && EnemyData.DropData.IsValid() && !bIsAlive)
	{
		const UEnemySettings* Settings = UEnemySettings::Get();
		const FGameplayTag& DropRewardTag = Settings->TagEvent_DropReward;

		UBroadcastSubsystem* BroadcastSubsystem = UBroadcastSubsystem::Get(GetWorld());
		if (IsValid(BroadcastSubsystem) && DropRewardTag.IsValid())
		{
			BroadcastSubsystem->BroadcastMessage(DropRewardTag, EnemyData.DropData);
		}
	}
}

