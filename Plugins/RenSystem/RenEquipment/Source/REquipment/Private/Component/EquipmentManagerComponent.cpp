// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Component/EquipmentManagerComponent.h"

// Engine Headers
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Engine/AssetManager.h"
#include "EnhancedInputComponent.h"
#include "StructUtils/InstancedStruct.h"
#include "UObject/ObjectSaveContext.h"

// Project Headers
#include "Actor/EquipmentActor.h"
#include "Core/AssetManagerUtil.h"
#include "Core/EquipmentSettings.h"
#include "Core/Interface/AssetInstanceContextProvider.h"
#include "Core/Type/EquipmentSpawnData.h"
#include "Data/CoreDataAsset.h"
#include "Data/EquipmentDataDefinition.h"
#include "Data/EquipmentFragment.h"
#include "Data/EquipmentInputMapping.h"
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

	if (SourceType == EDataSource::Runtime)
	{
		EquipmentSubsystem = UEquipmentSubsystem::Get(GetWorld());
		if (IsValid(EquipmentSubsystem))
		{
			EquipmentSubsystem->OnSyncEquipment.AddUObject(this, &UEquipmentManagerComponent::UpdateEquipment);
		}

		IAssetInstanceContextProvider* InstanceContext = GetOwner<IAssetInstanceContextProvider>();
		if (InstanceContext)
		{
			OwnerInstanceId = InstanceContext->GetAssetInstanceId();
		}
	}

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

	AssetManager = nullptr;
	ActorFreelist = nullptr;

	SetInitialized(false);
}


void UEquipmentManagerComponent::ActivateEquipmentById(FGameplayTag SlotTag, int Id)
{
	ActivateEquipmentById(FEquipmentSlotId(SlotTag, Id));
}

void UEquipmentManagerComponent::ActivateEquipmentById(const FEquipmentSlotId& SlotId)
{
	if (!IsInitialized())
	{
		return;
	}

	UEquipmentController* TargetController = nullptr;
	for (TObjectPtr<UEquipmentController> Controller : EquipmentControllers)
	{
		const FEquipmentInitializationData& EquipmentData = Controller->GetEquipmentData();
		if (EquipmentData.SlotId == SlotId)
		{
			TargetController = Controller;
			break;
		}
	}

	if (!IsValid(TargetController))
	{
		return;
	}

	bool bIsStateful = TargetController->IsA(UEquipmentStateController::StaticClass());
	if (!bIsStateful)
	{
		TargetController->ActivateEquipment();
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

UEquipmentController* UEquipmentManagerComponent::GetEquipmentControllerByTag(const FEquipmentSlotId& SlotId) const
{
	const TObjectPtr<UEquipmentController>* Controller = EquipmentControllers.FindByPredicate(
		[SlotId](UEquipmentController* Controller)
		{
			return (IsValid(Controller) && Controller->GetEquipmentData().SlotId == SlotId) == true;
		}
	);
	
	if (!Controller)
	{
		return nullptr;
	}
	return Controller->Get();
}

UEquipmentController* UEquipmentManagerComponent::GetActiveController() const
{
	return CurrentController;
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
	FAssetManagerUtil::CancelHandle(_SpawnHandle);
	OnEquipmentReset.Broadcast();

	SetInitialized(false);
	RemovePendingController();
	RemoveCurrentController();

	TArray<FPrimaryAssetId> AssetIds;
	RefreshEquipmentData(AssetIds);
	CleanupEquipmentData();

	if (AssetIds.Num() == 0)
	{
		SetInitialized(true);
		return;
	}

	const UEquipmentSettings* Settings = UEquipmentSettings::Get();
	const TArray<FName>& AssetBundles = Settings->EquipmentBundles;

	_SpawnHandle = AssetManager->LoadPrimaryAssets(AssetIds, AssetBundles, FStreamableDelegate::CreateUObject(this, &UEquipmentManagerComponent::SpawnEquipmentActors));
}

void UEquipmentManagerComponent::RemoveEquipment()
{
	FAssetManagerUtil::CancelHandle(_SpawnHandle);
	OnEquipmentReset.Broadcast();

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

	for (const FEquipmentInitializationData& Data : EquipmentSpawnData)
	{
		const UCoreDataAsset* Asset = AssetManager->GetPrimaryAssetObject<UCoreDataAsset>(Data.AssetId);
		bool bFoundController = EquipmentControllers.ContainsByPredicate([Data](UEquipmentController* Controller) { return IsValid(Controller) && Controller->GetEquipmentData() == Data; });
		if (!IsValid(Asset) || bFoundController)
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

		Controller->SourceType = SourceType;

		if (!Controller->InitializeController(Asset, Data, Actor, Definition))
		{
			LOG_ERROR(LogEquipment, TEXT("Failed to initialize controller, de-initializing it"));
			Controller->DeinitializeController();
			continue;
		}

		RegisterEquipment(Controller);
	}

	SetInitialized(true);
	OnEquipmentUpdate.Broadcast();
}


void UEquipmentManagerComponent::RefreshEquipmentData(TArray<FPrimaryAssetId>& OutAssetIds)
{
	if (SourceType == EDataSource::Static)
	{
		//ISpawnContextProvider* SpawnContext = GetOwner<ISpawnContextProvider>();
		//if (SpawnContext)
		//{
		//	const UEquipmentSettings* Settings = UEquipmentSettings::Get();

		//	FInstancedStruct EquipmentDataList;
		//	SpawnContext->GetSpawnData(Settings->EquipmentDataTag, EquipmentDataList);

		//	const FEquipmentInitializationDataList* DataList = EquipmentDataList.GetPtr<FEquipmentInitializationDataList>();
		//	if (DataList)
		//	{
		//		EquipmentSpawnData = DataList->EquipmentList;
		//	}
		//}
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

		StorageManager->GetEquipmentByOwnerId(OwnerInstanceId, EquipmentSpawnData);
	}

	for (const FEquipmentInitializationData& SpawnData : EquipmentSpawnData)
	{
		OutAssetIds.Add(SpawnData.AssetId);
	}
}

void UEquipmentManagerComponent::CleanupEquipmentData()
{
	//RemoveEquipment();

	for (int i = EquipmentControllers.Num() - 1; i >= 0; --i)
	{
		UEquipmentController* Controller = EquipmentControllers[i];
		if (IsValid(Controller) && !EquipmentSpawnData.Contains(Controller->GetEquipmentData()))
		{
			UnregisterEquipment(Controller);
			EquipmentControllers.RemoveAt(i);
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

	AEquipmentActor* Actor = Controller->GetEquipmentActor();
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
		OnEquipmentChanged.Broadcast();

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
	OnEquipmentChanged.Broadcast();

	PendingController = nullptr;
	BindController(Equipment);

	const bool bActivated = Equipment->ActivateEquipment();
	if (!bActivated)
	{
		if (CurrentController == Equipment)
		{
			CurrentController = nullptr;
			OnEquipmentChanged.Broadcast();
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
	OnEquipmentChanged.Broadcast();
}


void UEquipmentManagerComponent::BindController(UEquipmentStateController* Controller)
{
	UnbindController(Controller);

	if (IsValid(Controller))
	{
		Controller->OnActivatedDelegate.BindUObject(this, &UEquipmentManagerComponent::HandleOnEquipmentActivated);
		Controller->OnDeactivatedDelegate.BindUObject(this, &UEquipmentManagerComponent::HandleOnEquipmentDeactivated);
	}
}

void UEquipmentManagerComponent::UnbindController(UEquipmentStateController* Controller)
{
	if (IsValid(Controller))
	{
		Controller->OnActivatedDelegate.Unbind();
		Controller->OnDeactivatedDelegate.Unbind();
	}
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
		OnEquipmentChanged.Broadcast();

		UnbindController(Controller);

		PendingController = NextEquipment;
		ActivatePendingController();
		return;
	}

	CurrentController = nullptr;
	OnEquipmentChanged.Broadcast();

	UnbindController(Controller);
}

bool UEquipmentManagerComponent::IsInitialized() const
{
	return _bInitialized;
}

void UEquipmentManagerComponent::SetInitialized(bool bValue)
{
	_bInitialized = bValue;
}

