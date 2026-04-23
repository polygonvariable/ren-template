// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Component/EquipmentManagerComponent.h"

// Engine Headers
#include "InstancedStruct.h"
#include "UObject/ObjectSaveContext.h"

// Project Headers
#include "Actor/EquipmentActor.h"
#include "Asset/CoreDataAsset.h"
#include "Controller/EquipmentController.h"
#include "Definition/EquipmentData.h"
#include "Interface/Actor/AssetInstanceContextProvider.h"
#include "Interface/SpawnContextProvider.h"
#include "Library/PoolHelper.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Manager/RAssetManager.inl"
#include "Object/EquipmentMetadata.h"
#include "Settings/EquipmentSettings.h"
#include "Storage/EquipmentStorage.h"
#include "Subsystem/ActorFreelistSubsystem.h"
#include "Subsystem/EquipmentSubsystem.h"


UEquipmentManagerComponent::UEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	bAutoActivate = false;
	SetIsReplicatedByDefault(true);
}

void UEquipmentManagerComponent::BeginPlay()
{
	AssetManager = URAssetManager::Get();
	ActorFreelist = UActorFreelistSubsystem::Get(GetWorld());

	if (SourceType == EAssetQuerySource::Instance)
	{
		EquipmentSubsystem = UEquipmentSubsystem::Get(GetWorld());
		if (IsValid(EquipmentSubsystem))
		{
			EquipmentSubsystem->OnSyncEquipment.AddUObject(this, &UEquipmentManagerComponent::SyncEquipment);
			EquipmentStorage = EquipmentSubsystem->GetEquipmentStorage();
		}

		IAssetInstanceContextProvider* InstanceContext = GetOwner<IAssetInstanceContextProvider>();
		if (InstanceContext)
		{
			OwnerInstanceId = InstanceContext->GetAssetInstanceId();
		}
	}

	Super::BeginPlay();
}

void UEquipmentManagerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	RemoveEquipment();
	
	if (IsValid(EquipmentSubsystem))
	{
		EquipmentSubsystem->OnSyncEquipment.RemoveAll(this);
	}
	EquipmentStorage = nullptr;
	EquipmentSubsystem = nullptr;

	if (IsValid(AssetManager) && _SpawnId.IsValid())
	{
		AssetManager->CancelFetch(_SpawnId);
	}
	AssetManager = nullptr;
	ActorFreelist = nullptr;

	Super::EndPlay(EndPlayReason);
}

void UEquipmentManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UEquipmentManagerComponent::PreSave(FObjectPreSaveContext ObjectSaveContext)
{
	Super::PreSave(ObjectSaveContext);

#if WITH_EDITOR
	EquippedAssetIds.Empty();

	if (SourceType == EAssetQuerySource::Asset)
	{
		for (const FEquipmentData& Data : EquipmentSpawnData)
		{
			EquippedAssetIds.Add(Data.AssetId);
		}
	}
	else
	{
		EquipmentSpawnData.Empty();
	}
#endif
}

void UEquipmentManagerComponent::SyncEquipment(const FGuid& InOwnerId)
{
	if (InOwnerId == OwnerInstanceId)
	{
		SpawnEquipment();
	}
}


void UEquipmentManagerComponent::SpawnEquipment()
{
	OnEquipmentChangeBegin.Broadcast();

	RefreshEquipmentData();
	CleanupEquipmentData();

	if (EquippedAssetIds.Num() == 0)
	{
		OnEquipmentChangeEnd.Broadcast();
		return;
	}

	AssetManager->CancelFetch(_SpawnId);

	_SpawnId = FGuid::NewGuid();

	const TArray<FName>& AssetBundles = UEquipmentSettings::Get()->EquipmentBundles;

	TWeakObjectPtr<UEquipmentManagerComponent> WeakThis(this);
	TFuture<FLatentLoadedAssets<UCoreDataAsset>> Future = AssetManager->FetchPrimaryAssets<UCoreDataAsset>(_SpawnId, EquippedAssetIds, AssetBundles, false);
	Future.Next([WeakThis](const FLatentLoadedAssets<UCoreDataAsset>& Result)
		{
			UEquipmentManagerComponent* This = WeakThis.Get();
			if (IsValid(This) && Result.IsCompleted())
			{
				This->SpawnEquipmentActors();
			}
		}
	);
}

void UEquipmentManagerComponent::RemoveEquipment()
{
	TArray<FEquipmentData> CurrentData;
	EquippedControllers.GetKeys(CurrentData);

	for (const FEquipmentData& Data : CurrentData)
	{
		UnregisterEquipment(Data);
	}

	EquippedControllers.Empty();
}

UEquipmentController* UEquipmentManagerComponent::GetEquipmentControllerByTag(const FGameplayTag& Tag) const
{
	for (const TPair<FEquipmentData, UEquipmentController*>& Kv : EquippedControllers)
	{
		if (Kv.Key.EquipmentSlot.MatchesTag(Tag))
		{
			return Kv.Value;
		}
	}
	return nullptr;
}

void UEquipmentManagerComponent::SpawnEquipmentActors()
{
	UWorld* World = GetWorld();

	for (const FEquipmentData& Data : EquipmentSpawnData)
	{
		const UCoreDataAsset* Asset = AssetManager->GetPrimaryAssetObject<UCoreDataAsset>(Data.AssetId);
		if (!IsValid(Asset) || EquippedControllers.Contains(Data))
		{
			LOG_ERROR(LogEquipment, TEXT("Asset is invalid or already spawned"));
			continue;
		}

		const UEquipmentFragment* Fragment = Asset->FindFragmentByClass<UEquipmentFragment>();
		if (!IsValid(Fragment))
		{
			LOG_ERROR(LogEquipment, TEXT("Equipment fragment is invalid"));
			continue;
		}

		const UEquipmentDataDefinition* DefinitionCDO = Fragment->GetEquipmentDefinitionCDO();
		if (!IsValid(DefinitionCDO))
		{
			LOG_ERROR(LogEquipment, TEXT("Equipment data definition is invalid"));
			continue;
		}

		UClass* ControllerClass = DefinitionCDO->ControllerClass.Get();
		UClass* ActorClass = DefinitionCDO->ActorClass.Get();
		if (!IsValid(ControllerClass) || !IsValid(ActorClass))
		{
			LOG_ERROR(LogEquipment, TEXT("Controller or actor class is invalid"));
			continue;
		}

		UEquipmentController* Controller = FPoolHelper::Acquire<UEquipmentController>(_ControllerPool, ControllerClass, this);
		AEquipmentActor* Actor = ActorFreelist->AcquireFromList<AEquipmentActor>(ActorClass, FTransform(), GetOwner());
		if (!IsValid(Controller) || !IsValid(Actor))
		{
			LOG_ERROR(LogEquipment, TEXT("Controller or actor is invalid"));
			continue;
		}

		if (!Actor->HasActorBegunPlay())
		{
			Actor->FinishSpawning(FTransform());
		}

		Controller->EquipmentActor = Actor;
		Controller->EquipmentAsset = Asset;
		Controller->EquipmentData = Data;

		if (!Controller->InitializeController(DefinitionCDO))
		{
			LOG_ERROR(LogEquipment, TEXT("Failed to initialize controller"));
			continue;
		}

		RegisterEquipment(Data, Controller);
	}

	OnEquipmentChangeEnd.Broadcast();
}

void UEquipmentManagerComponent::RefreshEquipmentData()
{
	if (SourceType == EAssetQuerySource::Asset)
	{
		ISpawnContextProvider* SpawnContext = GetOwner<ISpawnContextProvider>();
		if (SpawnContext)
		{
			const UEquipmentSettings* Settings = UEquipmentSettings::Get();

			FInstancedStruct EquipmentDataList;
			SpawnContext->GetSpawnData(Settings->EquipmentDataTag, EquipmentDataList);

			const FEquipmentDataList* DataList = EquipmentDataList.GetPtr<FEquipmentDataList>();
			if (DataList)
			{
				EquipmentSpawnData = DataList->EquipmentList;

				EquippedAssetIds.Empty();
				for (const FEquipmentData& Data : EquipmentSpawnData)
				{
					EquippedAssetIds.Add(Data.AssetId);
				}
			}
		}
	}
	else
	{
		EquipmentSpawnData.Empty();
		EquippedAssetIds.Empty();

		if (!IsValid(EquipmentStorage))
		{
			LOG_ERROR(LogEquipment, TEXT("EquipmentStorage is invalid"));
			return;
		}

		const TMap<FGameplayTag, FEquipmentKey>* OwnedEquipment = EquipmentStorage->GetOwnedEquipment(OwnerInstanceId);
		if (!OwnedEquipment)
		{
			LOG_ERROR(LogEquipment, TEXT("OwnedEquipment is invalid"));
			return;
		}

		for (const TPair<FGameplayTag, FEquipmentKey>& Kv : *OwnedEquipment)
		{
			const FEquipmentKey& EquipmentKey = Kv.Value;

			FEquipmentData Data;
			Data.AssetId = EquipmentKey.AssetId;
			Data.EquipmentId = EquipmentKey.AssetInstanceId;
			Data.EquipmentSlot = Kv.Key;
			Data.SourceType = EAssetQuerySource::Instance;

			EquipmentSpawnData.Add(Data);
			EquippedAssetIds.Add(Data.AssetId);
		}
	}
}

void UEquipmentManagerComponent::CleanupEquipmentData()
{
	TArray<FEquipmentData> CurrentData;
	EquippedControllers.GetKeys(CurrentData);
	
	for (const FEquipmentData& Data : CurrentData)
	{
		if (!EquipmentSpawnData.Contains(Data))
		{
			UnregisterEquipment(Data);
		}
	}
}

void UEquipmentManagerComponent::RegisterEquipment(const FEquipmentData& Data, UEquipmentController* Controller)
{
	EquippedControllers.Add(Data, Controller);
}

void UEquipmentManagerComponent::UnregisterEquipment(const FEquipmentData& Data)
{
	UEquipmentController* Controller = EquippedControllers.FindAndRemoveChecked(Data);
	if (!IsValid(Controller))
	{
		return;
	}

	AEquipmentActor* Actor = Controller->EquipmentActor;
	if (!IsValid(Actor))
	{
		return;
	}

	Controller->DeinitializeController();

	FPoolHelper::Return(_ControllerPool, Controller);
	ActorFreelist->ReturnToList(Actor);
}

bool UEquipmentManagerComponent::GetIsSpawning() const
{
	return _bIsSpawning;
}

void UEquipmentManagerComponent::SetIsSpawning(bool bIsSpawning)
{
	_bIsSpawning = bIsSpawning;
}

