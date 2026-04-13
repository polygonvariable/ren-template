// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Component/EquipmentManagerComponent.h"

// Engine Headers
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "InstancedStruct.h"
#include "UObject/ObjectSaveContext.h"

// Project Headers
#include "Actor/EquipmentActor.h"
#include "Asset/CoreDataAsset.h"
#include "Asset/EquipmentDataAsset.h"
#include "Definition/EquipmentData.h"
#include "Interface/Actor/AssetInstanceContextProvider.h"
#include "Interface/EquipmentDataProvider.h"
#include "Interface/SpawnContextProvider.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Manager/RAssetManager.inl"
#include "Settings/EquipmentSettings.h"
#include "Storage/EquipmentStorage.h"
#include "Subsystem/ActorFreeListSubsystem.h"
#include "Subsystem/EquipmentSubsystem.h"
#include "Object/EquipmentMetadata.h"


UEquipmentManagerComponent::UEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	bAutoActivate = false;
	SetIsReplicatedByDefault(true);
}

void UEquipmentManagerComponent::BeginPlay()
{
	AssetManager = URAssetManager::Get();
	ActorFreeList = UActorFreeListSubsystem::Get(GetWorld());

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
			EquipmentOwnerId = InstanceContext->GetAssetInstanceId();
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
	ActorFreeList = nullptr;

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
	if (InOwnerId == EquipmentOwnerId)
	{
		SpawnEquipment();
	}
}

void UEquipmentManagerComponent::SpawnEquipment()
{
	RefreshEquipmentData();
	CleanupEquipmentActors();

	if (EquippedAssetIds.Num() == 0)
	{
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
	//for (const TPair<FEquipmentData, TObjectPtr<AEquipmentActor>>& Kv : EquippedActors)
	//{
	//	AEquipmentActor* Actor = Kv.Value;
	//	if (IsValid(Actor))
	//	{
	//		Actor->DeinitializeEquipment();
	//		ActorFreeList->ReturnToList(Actor);
	//	}
	//}

	//EquippedActors.Empty();
}

void UEquipmentManagerComponent::SpawnEquipmentActors()
{
	UWorld* World = GetWorld();
	FTransform SpawnTransform;


	for (const FEquipmentData& Data : EquipmentSpawnData)
	{
		UCoreDataAsset* Asset = AssetManager->GetPrimaryAssetObject<UCoreDataAsset>(Data.AssetId);
		const IEquipmentDataProvider* EquipmentData = Cast<IEquipmentDataProvider>(Asset);
		if (!EquipmentData || EquippedControllers.Contains(Data))
		{
			LOG_ERROR(LogEquipment, TEXT("Asset does not implement IEquipmentDataProvider or is already active"));
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
			LOG_ERROR(LogEquipment, TEXT("Class is invalid or not child of EquipmentActor"));
			continue;
		}

		UEquipmentController* Controller = NewObject<UEquipmentController>(this, ControllerClass);
		AEquipmentActor* Actor = ActorFreeList->AcquireFromList<AEquipmentActor>(ActorClass, SpawnTransform, GetOwner());
		if (!IsValid(Controller) || !IsValid(Actor))
		{
			LOG_ERROR(LogEquipment, TEXT("Controller or actor is invalid"));
			return;
		}

		if (!Actor->HasActorBegunPlay())
		{
			Actor->FinishSpawning(SpawnTransform);
		}

		Controller->EquipmentActor = Actor;
		Controller->EquipmentAsset = Asset;
		Controller->EquipmentData = Data;
		Controller->EquipmentOwnerId = EquipmentOwnerId;

		Controller->InitializeController(DefinitionCDO);

		//Actor->EquipmentAsset = Asset;
		//Actor->EquipmentData = Data;
		//Actor->EquipmentOwnerId = EquipmentOwnerId;

		//Actor->InitializeEquipment();


		RegisterEquipment(Data, Controller);
	}









	//for (const FEquipmentData& Data : EquipmentSpawnData)
	//{
	//	UCoreDataAsset* CoreDataAsset = AssetManager->GetPrimaryAssetObject<UCoreDataAsset>(Data.AssetId);
	//	const IEquipmentDataProvider* EquipmentData = Cast<IEquipmentDataProvider>(CoreDataAsset);
	//	if (!EquipmentData || EquippedActors.Contains(Data))
	//	{
	//		LOG_ERROR(LogEquipment, TEXT("Asset does not implement IEquipmentDataProvider or is already active"));
	//		continue;
	//	}

	//	const UEquipmentFragment* EquipmentFragment = CoreDataAsset->FindFragmentByClass<UEquipmentFragment>();
	//	if (!IsValid(EquipmentFragment))
	//	{
	//		LOG_ERROR(LogEquipment, TEXT("Equipment fragment is invalid"));
	//		continue;
	//	}

	//	const UEquipmentMetadata* EquipmentMetadata = EquipmentFragment->GetMetadata();
	//	if (!IsValid(EquipmentMetadata))
	//	{
	//		LOG_ERROR(LogEquipment, TEXT("Equipment metadata is invalid"));
	//		continue;
	//	}

	//	UClass* EquipmentClass = EquipmentMetadata->EquipmentClass.Get();
	//	if (!IsValid(EquipmentClass) || !EquipmentClass->IsChildOf(AEquipmentActor::StaticClass()))
	//	{
	//		LOG_ERROR(LogEquipment, TEXT("Class is invalid or not child of EquipmentActor"));
	//		continue;
	//	}

	//	AEquipmentActor* EquipmentActor = ActorFreeList->AcquireFromList<AEquipmentActor>(EquipmentClass, SpawnTransform, GetOwner());
	//	if (!IsValid(EquipmentActor))
	//	{
	//		LOG_ERROR(LogEquipment, TEXT("Spawned character is invalid"));
	//		return;
	//	}

	//	EquipmentActor->EquipmentAsset = CoreDataAsset;
	//	EquipmentActor->EquipmentData = Data;
	//	EquipmentActor->EquipmentOwnerId = EquipmentOwnerId;

	//	EquipmentActor->InitializeEquipment();

	//	if (!EquipmentActor->HasActorBegunPlay())
	//	{
	//		EquipmentActor->FinishSpawning(SpawnTransform);
	//	}

	//	RegisterEquipment(Data, EquipmentActor);
	//}
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

		const TMap<FGameplayTag, FEquipmentKey>* OwnedEquipment = EquipmentStorage->GetOwnedEquipment(EquipmentOwnerId);
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

void UEquipmentManagerComponent::CleanupEquipmentActors()
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

	Actor->DeinitializeEquipment();
	ActorFreeList->ReturnToList(Actor);
}

