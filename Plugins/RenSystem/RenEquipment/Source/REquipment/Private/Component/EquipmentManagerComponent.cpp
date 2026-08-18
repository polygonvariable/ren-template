// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Component/EquipmentManagerComponent.h"

// Engine Headers
#include "AbilitySystemBlueprintLibrary.h"
#include "Engine/AssetManager.h"
#include "StructUtils/InstancedStruct.h"
#include "UObject/ObjectSaveContext.h"

// Project Headers
#include "Actor/EquipmentActor.h"
#include "Core/AssetManagerUtil.h"
#include "Core/EquipmentSettings.h"
#include "Core/Interface/AssetInstanceContextProvider.h"
#include "Core/Type/EquipmentData.h"
#include "Data/CoreDataAsset.h"
#include "Data/EquipmentDataDefinition.h"
#include "Data/EquipmentFragment.h"
#include "Interface/SpawnContextProvider.h"
#include "Library/PoolHelper.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Subsystem/ActorFreelistSubsystem.h"
#include "System/Controller/EquipmentStateController.h"
#include "System/EquipmentController.h"
#include "System/EquipmentStorageManager.h"
#include "System/EquipmentSubsystem.h"


UEquipmentManagerComponent::UEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	bAutoActivate = false;
	SetIsReplicatedByDefault(true);
}


void UEquipmentManagerComponent::InitializeManager()
{
	AssetManager = UAssetManager::GetIfInitialized();
	ActorFreelist = UActorFreelistSubsystem::Get(GetWorld());

	//ISpawnContextProvider* SpawnContext = GetOwner<ISpawnContextProvider>();
	//if (SpawnContext)
	//{
	//	SourceType = SpawnContext->GetSpawnSource();
	//}

	//if (SourceType == EDataSource::Runtime)
	//{
	//	EquipmentSubsystem = UEquipmentSubsystem::Get(GetWorld());
	//	if (IsValid(EquipmentSubsystem))
	//	{
	//		EquipmentSubsystem->OnSyncEquipment.AddUObject(this, &UEquipmentManagerComponent::UpdateEquipment);
	//	}

	//	IAssetInstanceContextProvider* InstanceContext = GetOwner<IAssetInstanceContextProvider>();
	//	if (InstanceContext)
	//	{
	//		OwnerInstanceId = InstanceContext->GetAssetInstanceId();
	//	}
	//}

	CreateEquipment();
}

void UEquipmentManagerComponent::DeinitializeManager()
{
	RemoveEquipment();

	if (IsValid(EquipmentSubsystem))
	{
		EquipmentSubsystem->OnSyncEquipment.RemoveAll(this);
	}
	EquipmentSubsystem = nullptr;

	FAssetManagerUtil::CancelHandle(_SpawnHandle);
	AssetManager = nullptr;
	ActorFreelist = nullptr;

	SetInitialized(false);
}


void UEquipmentManagerComponent::ActivateEquipmentById(FGameplayTag CategoryTag, int SlotId)
{
	if (!IsInitialized())
	{
		return;
	}

	UEquipmentController* TargetController = nullptr;
	for (TObjectPtr<UEquipmentController> Controller : EquipmentControllers)
	{
		const FEquipmentData& EquipmentData = Controller->EquipmentData;
		if (EquipmentData.CategoryTag == CategoryTag && EquipmentData.SlotId == SlotId)
		{
			TargetController = Controller;
			break;
		}
	}

	if (!IsValid(TargetController))
	{
		return;
	}

	if (!TargetController->IsA(UEquipmentStateController::StaticClass()))
	{
		return;
	}

	UEquipmentStateController* StateController = Cast<UEquipmentStateController>(TargetController);
	if (!CurrentController)
	{
		PendingController = Cast<UEquipmentStateController>(StateController);
		ActivatePendingController();
		return;
	}

	if (StateController == CurrentController)
	{
		switch (StateController->GetState())
		{
		case EEquipmentState::Inactive:
		{
			RemovePendingController();
			ActivatePendingController();
			return;
		}
		case EEquipmentState::Activating:
		{
			RemovePendingController();
			return;
		}
		case EEquipmentState::Active:
		{
			RemovePendingController();
			StateController->DeactivateEquipment();
			return;
		}
		case EEquipmentState::Deactivating:
		{
			RemovePendingController();
			return;
		}
		default:
			break;
		}
	}

	PendingController = StateController;
	HandleControllerQueue();
}

UEquipmentController* UEquipmentManagerComponent::GetEquipmentControllerByTag(const FGameplayTag& Tag) const
{
	//for (const TPair<FEquipmentData, UEquipmentController*>& Kv : EquippedControllers)
	//{
	//	if (Kv.Key.EquipmentSlot.MatchesTag(Tag))
	//	{
	//		return Kv.Value;
	//	}
	//}
	return nullptr;
}


void UEquipmentManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UEquipmentManagerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	DeinitializeManager();
	Super::EndPlay(EndPlayReason);
}

void UEquipmentManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UEquipmentManagerComponent::PreSave(FObjectPreSaveContext ObjectSaveContext)
{
	Super::PreSave(ObjectSaveContext);
}


void UEquipmentManagerComponent::UpdateEquipment(const FGuid& InOwnerId)
{
	if (InOwnerId == OwnerInstanceId)
	{
		CreateEquipment();
	}
}

void UEquipmentManagerComponent::CreateEquipment()
{
	OnEquipmentSpawnBegin.Broadcast();

	SetInitialized(false);
	RemovePendingController();
	RemoveCurrentController();

	TArray<FPrimaryAssetId> AssetIds;
	RefreshEquipmentData(AssetIds);
	CleanupEquipmentData();

	if (AssetIds.Num() == 0)
	{
		SetInitialized(true);
		OnEquipmentSpawnEnd.Broadcast();
		return;
	}

	FAssetManagerUtil::CancelHandle(_SpawnHandle);

	const UEquipmentSettings* Settings = UEquipmentSettings::Get();
	const TArray<FName>& AssetBundles = Settings->EquipmentBundles;

	_SpawnHandle = AssetManager->LoadPrimaryAssets(AssetIds, AssetBundles, FStreamableDelegate::CreateUObject(this, &UEquipmentManagerComponent::SpawnEquipmentActors));
}

void UEquipmentManagerComponent::RemoveEquipment()
{
	RemovePendingController();
	RemoveCurrentController();

	for (TObjectPtr<UEquipmentController> Controller : EquipmentControllers)
	{
		UnregisterEquipment(Controller);
	}

	EquipmentControllers.Empty();
}

void UEquipmentManagerComponent::SpawnEquipmentActors()
{
	FAssetManagerUtil::ReleaseHandle(_SpawnHandle);

	UWorld* World = GetWorld();

	for (const FEquipmentData& SpawnData : EquipmentSpawnData)
	{
		const UCoreDataAsset* Asset = AssetManager->GetPrimaryAssetObject<UCoreDataAsset>(SpawnData.AssetId);
		if (!IsValid(Asset))
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

		const UEquipmentDataDefinition* Definition = Fragment->GetEquipmentDefinition();
		if (!IsValid(Definition))
		{
			LOG_ERROR(LogEquipment, TEXT("Equipment data definition is invalid"));
			continue;
		}

		UClass* ControllerClass = Definition->ControllerClass.Get();
		UClass* ActorClass = Definition->ActorClass.Get();
		if (!IsValid(ControllerClass) || !IsValid(ActorClass))
		{
			LOG_ERROR(LogEquipment, TEXT("Controller or actor class is invalid"));
			continue;
		}

		UEquipmentController* Controller = FPoolHelper::AcquireFromContainer<UEquipmentController>(_ControllerPool, ControllerClass, this);
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
		Controller->EquipmentData = SpawnData;

		if (!Controller->InitializeController(Definition))
		{
			LOG_ERROR(LogEquipment, TEXT("Failed to initialize controller, de-initializing it"));
			Controller->DeinitializeController();
			continue;
		}

		RegisterEquipment(Controller);
	}

	SetInitialized(true);
	OnEquipmentSpawnEnd.Broadcast();
}


void UEquipmentManagerComponent::RefreshEquipmentData(TArray<FPrimaryAssetId>& OutAssetIds)
{
	if (SourceType == EDataSource::Static)
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
			}
		}

		for (const FEquipmentData& SpawnData : EquipmentSpawnData)
		{
			OutAssetIds.Add(SpawnData.AssetId);
		}
	}
	else
	{
		EquipmentSpawnData.Empty();

		if (!IsValid(EquipmentSubsystem))
		{
			LOG_ERROR(LogEquipment, TEXT("EquipmentSubsystem is invalid"));
			return;
		}

		UEquipmentStorageManager* StorageManager = EquipmentSubsystem->GetStorageManager();
		if (!IsValid(StorageManager))
		{
			LOG_ERROR(LogEquipment, TEXT("StorageManager is invalid"));
			return;
		}

		const TMap<FGameplayTag, FEquipmentKey>* OwnedEquipment = StorageManager->GetOwnedEquipment(OwnerInstanceId);
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
			Data.AssetInstanceId = EquipmentKey.AssetInstanceId;
			Data.CategoryTag = Kv.Key;
			Data.SourceType = SourceType;

			EquipmentSpawnData.Add(Data);
			OutAssetIds.Add(Data.AssetId);
		}
	}
}

void UEquipmentManagerComponent::CleanupEquipmentData()
{
	for (TObjectPtr<UEquipmentController> Controller : EquipmentControllers)
	{
		if (IsValid(Controller))
		{
			if (!EquipmentSpawnData.Contains(Controller->EquipmentData))
			{
				UnregisterEquipment(Controller);
			}
		}
	}
}

void UEquipmentManagerComponent::RegisterEquipment(UEquipmentController* Controller)
{
	EquipmentControllers.Add(Controller);
}

void UEquipmentManagerComponent::UnregisterEquipment(UEquipmentController* Controller)
{
	if (!IsValid(Controller))
	{
		return;
	}

	AEquipmentActor* Actor = Controller->EquipmentActor;
	if (!IsValid(Actor))
	{
		return;
	}

	UnbindController(Cast<UEquipmentStateController>(Controller));
	Controller->DeinitializeController();

	FPoolHelper::ReturnToContainer(_ControllerPool, Controller);
	ActorFreelist->ReturnToList(Actor);
}


void UEquipmentManagerComponent::HandleControllerQueue()
{
	if (!IsInitialized())
	{
		return;
	}

	if (!PendingController)
	{
		return;
	}

	if (!CurrentController)
	{
		ActivatePendingController();
		return;
	}

	if (CurrentController->IsActivating() || CurrentController->IsDeactivating())
	{
		return;
	}

	if (CurrentController->IsActive())
	{
		CurrentController->DeactivateEquipment();
		return;
	}

	if (CurrentController->IsInactive())
	{
		UEquipmentStateController* PreviousEquipment = CurrentController;

		CurrentController = nullptr;

		UnbindController(PreviousEquipment);
		ActivatePendingController();
	}
}

void UEquipmentManagerComponent::ActivatePendingController()
{
	if (!IsInitialized())
	{
		RemovePendingController();
		RemoveCurrentController();
		return;
	}

	if (!PendingController)
	{
		return;
	}

	UEquipmentStateController* Equipment = PendingController;
	if (Equipment == CurrentController)
	{
		PendingController = nullptr;

		if (Equipment->IsActive())
		{
			return;
		}

		if (Equipment->IsActivating() || Equipment->IsDeactivating())
		{
			return;
		}
	}

	CurrentController = Equipment;
	PendingController = nullptr;
	BindController(Equipment);

	const bool bActivated = Equipment->ActivateEquipment();
	if (!bActivated)
	{
		if (CurrentController == Equipment)
		{
			CurrentController = nullptr;
		}
		UnbindController(Equipment);
		return;
	}

}

void UEquipmentManagerComponent::RemovePendingController()
{
	PendingController = nullptr;
}

void UEquipmentManagerComponent::RemoveCurrentController()
{
	CurrentController = nullptr;
}


void UEquipmentManagerComponent::BindController(UEquipmentStateController* Controller)
{
	UnbindController(Controller);

	Controller->OnActivatedDelegate.BindUObject(this, &UEquipmentManagerComponent::HandleOnEquipmentActivated);
	Controller->OnDeactivatedDelegate.BindUObject(this, &UEquipmentManagerComponent::HandleOnEquipmentDeactivated);
}

void UEquipmentManagerComponent::UnbindController(UEquipmentStateController* Controller)
{
	Controller->OnActivatedDelegate.Unbind();
	Controller->OnDeactivatedDelegate.Unbind();
}


void UEquipmentManagerComponent::HandleOnEquipmentActivated(UEquipmentStateController* Controller)
{
	if (Controller != CurrentController)
	{
		return;
	}

	if (PendingController)
	{
		if (PendingController == CurrentController)
		{
			PendingController = nullptr;
			return;
		}

		Controller->DeactivateEquipment();
		return;
	}
}

void UEquipmentManagerComponent::HandleOnEquipmentDeactivated(UEquipmentStateController* Controller)
{
	if (Controller != CurrentController)
	{
		return;
	}

	if (PendingController)
	{
		UEquipmentStateController* NextEquipment = PendingController;
		PendingController = nullptr;

		CurrentController = nullptr;
		UnbindController(Controller);

		PendingController = NextEquipment;
		ActivatePendingController();
		return;
	}

	CurrentController = nullptr;
	UnbindController(Controller);
}

