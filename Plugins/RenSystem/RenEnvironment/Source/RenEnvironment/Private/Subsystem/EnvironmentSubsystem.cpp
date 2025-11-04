// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Subsystem/EnvironmentSubsystem.h"

// Engine Headers
#include "EngineUtils.h"

// Project Headers
#include "RCoreAssetManager/Public/RAssetManager.h"
#include "RCoreAssetManager/Private/RAssetManager.inl"

#include "RCoreLibrary/Public/LogCategory.h"
#include "RCoreLibrary/Public/LogMacro.h"
#include "RCoreSettings/Public/WorldConfigSettings.h"

#include "RenEnvironment/Public/Asset/EnvironmentAsset.h"
#include "RenEnvironment/Public/Asset/EnvironmentProfileAsset.h"
#include "RenEnvironment/Public/Controller/EnvironmentController.h"



AActor* UEnvironmentSubsystem::GetEnvironmentActor()
{
	if (!EnvironmentActor.IsValid())
	{
		for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			if (IsValid(*ActorItr) && ActorItr->Tags.Contains(EnvironmentTag))
			{
				UE_LOG(LogEnvironment, Log, TEXT("EnvironmentActor: %s"), *ActorItr->GetName());
				EnvironmentActor = *ActorItr;
				break;
			}
		}
	}
	return EnvironmentActor.Get();
}

bool UEnvironmentSubsystem::AddProfile(UEnvironmentProfileAsset* ProfileAsset, int Priority)
{
	if (!IsValid(ProfileAsset))
	{
		LOG_ERROR(LogEnvironment, TEXT("ProfileAsset is invalid"));
		return false;
	}
	
	TObjectPtr<UEnvironmentStackedController>* Found = StackedControllers.Find(ProfileAsset->ProfileType);
	if (!Found)
	{
		LOG_ERROR(LogEnvironment, TEXT("EnvironmentStackedController is invalid"));
		return false;
	}

	UEnvironmentStackedController* Controller = Found->Get();
	if (!IsValid(Controller))
	{
		LOG_ERROR(LogEnvironment, TEXT("EnvironmentStackedController is invalid"));
		return false;
	}

	return Controller->AddItem(ProfileAsset, Priority);
}

bool UEnvironmentSubsystem::RemoveProfile(UEnvironmentProfileAsset* ProfileAsset, int Priority)
{
	if (!IsValid(ProfileAsset))
	{
		LOG_ERROR(LogEnvironment, TEXT("ProfileAsset is invalid"));
		return false;
	}

	EEnvironmentProfileType ProfileType = ProfileAsset->ProfileType;
	return RemoveProfile(ProfileType, Priority);
}

void UEnvironmentSubsystem::AddProfile(const FGuid& LatentId, const FPrimaryAssetId& AssetId, int Priority)
{
	if (!LatentId.IsValid() || !UEnvironmentProfileAsset::IsValid(AssetId) || !IsValid(AssetManager))
	{
		LOG_ERROR(LogEnvironment, TEXT("LatentId, AssetId, AssetManager is invalid"));
		return;
	}

	TFuture<FLatentResultAsset<UEnvironmentProfileAsset>> Future = AssetManager->FetchPrimaryAsset<UEnvironmentProfileAsset>(LatentId, AssetId);
	if (!Future.IsValid())
	{
		LOG_ERROR(LogWeather, TEXT("Failed to create Future"));
		return;
	}

	TWeakObjectPtr<UEnvironmentSubsystem> WeakThis(this);
	Future.Next([WeakThis, Priority](const FLatentResultAsset<UEnvironmentProfileAsset>& Result)
		{
			UEnvironmentSubsystem* This = WeakThis.Get();
			if (IsValid(This) && Result.IsValid())
			{
				This->AddProfile(Result.Asset, Priority);
			}
		}
	);
}

bool UEnvironmentSubsystem::RemoveProfile(const FGuid& LatentId, const FPrimaryAssetId& AssetId, int Priority)
{
	if (!LatentId.IsValid() || !UEnvironmentProfileAsset::IsValid(AssetId) || !IsValid(AssetManager))
	{
		LOG_ERROR(LogEnvironment, TEXT("LatentId, AssetId, AssetManager is invalid"));
		return false;
	}

	AssetManager->CancelFetch(LatentId);

	FAssetData AssetData;
	bool bSuccess = AssetManager->GetPrimaryAssetData(AssetId, AssetData);
	if (!bSuccess)
	{
		LOG_ERROR(LogEnvironment, TEXT("AssetData is invalid"));
		return false;
	}

	EEnvironmentProfileType ProfileType = EEnvironmentProfileType::Default;
	UEnvironmentProfileAsset::GetType(AssetData, ProfileType);

	return RemoveProfile(ProfileType, Priority);
}

void UEnvironmentSubsystem::AddProfile(const FPrimaryAssetId& AssetId, int Priority)
{
	if (!UEnvironmentProfileAsset::IsValid(AssetId) || !IsValid(AssetManager))
	{
		LOG_ERROR(LogEnvironment, TEXT("AssetId, AssetManager is invalid"));
		return;
	}

	TFuture<FLatentResultAsset<UEnvironmentProfileAsset>> Future = AssetManager->FetchPrimaryAsset<UEnvironmentProfileAsset>(AssetId);
	if (!Future.IsValid())
	{
		LOG_ERROR(LogWeather, TEXT("Failed to create Future"));
		return;
	}

	TWeakObjectPtr<UEnvironmentSubsystem> WeakThis(this);
	Future.Next([WeakThis, Priority](const FLatentResultAsset<UEnvironmentProfileAsset>& Result)
		{
			UEnvironmentSubsystem* This = WeakThis.Get();
			if (IsValid(This) && Result.IsValid())
			{
				This->AddProfile(Result.Asset, Priority);
			}
		}
	);
}

bool UEnvironmentSubsystem::RemoveProfile(const FPrimaryAssetId& AssetId, int Priority)
{
	if (!UEnvironmentProfileAsset::IsValid(AssetId) || !IsValid(AssetManager))
	{
		LOG_ERROR(LogEnvironment, TEXT("AssetId, AssetManager is invalid"));
		return false;
	}

	FAssetData AssetData;
	bool bSuccess = AssetManager->GetPrimaryAssetData(AssetId, AssetData);
	if (!bSuccess)
	{
		LOG_ERROR(LogEnvironment, TEXT("AssetData is invalid"));
		return false;
	}

	EEnvironmentProfileType ProfileType = EEnvironmentProfileType::Default;
	UEnvironmentProfileAsset::GetType(AssetData, ProfileType);

	return RemoveProfile(ProfileType, Priority);
}

bool UEnvironmentSubsystem::RemoveProfile(EEnvironmentProfileType ProfileType, int Priority)
{
	if (ProfileType == EEnvironmentProfileType::Default)
	{
		LOG_ERROR(LogEnvironment, TEXT("ProfileType is invalid"));
		return false;
	}

	TObjectPtr<UEnvironmentStackedController>* Controller = StackedControllers.Find(ProfileType);
	if (!Controller)
	{
		LOG_ERROR(LogEnvironment, TEXT("EnvironmentStackedController is invalid"));
		return false;
	}

	UEnvironmentStackedController* ControllerObject = Controller->Get();
	if (!IsValid(ControllerObject))
	{
		LOG_ERROR(LogEnvironment, TEXT("EnvironmentStackedController is invalid"));
		return false;
	}

	return ControllerObject->RemoveItem(Priority);
}



bool UEnvironmentSubsystem::RegisterStackedController(TSubclassOf<UEnvironmentStackedController> ControllerClass)
{
	if (!IsValid(ControllerClass))
	{
		LOG_ERROR(LogEnvironment, TEXT("ControllerClass is invalid"));
		return false;
	}

	UEnvironmentStackedController* DefaultObject = ControllerClass->GetDefaultObject<UEnvironmentStackedController>();
	if (StackedControllers.Contains(DefaultObject->ProfileType))
	{
		LOG_ERROR(LogEnvironment, TEXT("EnvironmentStackedController already exists"));
		return false;
	}

	UEnvironmentStackedController* NewController = NewObject<UEnvironmentStackedController>(this, ControllerClass);
	if (!IsValid(NewController))
	{
		LOG_ERROR(LogEnvironment, TEXT("EnvironmentStackedController is invalid"));
		return false;
	}

	NewController->InitializeController(GetEnvironmentActor());
	StackedControllers.Add(DefaultObject->ProfileType, NewController);

	return true;
}

bool UEnvironmentSubsystem::RegisterDiscreteController(TSubclassOf<UEnvironmentDiscreteController> ControllerClass)
{
	if (!IsValid(ControllerClass))
	{
		LOG_ERROR(LogEnvironment, TEXT("ControllerClass is invalid"));
		return false;
	}

	UEnvironmentDiscreteController* NewController = NewObject<UEnvironmentDiscreteController>(this, ControllerClass);
	if (!IsValid(NewController))
	{
		LOG_ERROR(LogEnvironment, TEXT("EnvironmentDiscreteController is invalid"));
		return false;
	}

	NewController->InitializeController(GetEnvironmentActor());
	DiscreateControllers.Add(NewController);

	return true;
}

void UEnvironmentSubsystem::LoadDefaultProfiles(const TArray<FPrimaryAssetId>& AssetIds)
{
	if (!IsValid(AssetManager) || AssetIds.IsEmpty())
	{
		LOG_ERROR(LogEnvironment, TEXT("AssetManager is invalid or AssetIds is empty"));
		return;
	}

	TFuture<FLatentResultAssets<UEnvironmentProfileAsset>> Future = AssetManager->FetchPrimaryAssets<UEnvironmentProfileAsset>(AssetIds);
	if (!Future.IsValid())
	{
		LOG_ERROR(LogWeather, TEXT("Failed to create Future"));
		return;
	}

	TWeakObjectPtr<UEnvironmentSubsystem> WeakThis(this);
	Future.Next([WeakThis](const FLatentResultAssets<UEnvironmentProfileAsset>& Result)
		{
			UEnvironmentSubsystem* This = WeakThis.Get();
			if (IsValid(This) && Result.IsValid())
			{
				for (UEnvironmentProfileAsset* Asset : Result.Assets)
				{
					This->AddProfile(Asset, 0);
				}
			}
		}
	);
}

bool UEnvironmentSubsystem::DoesSupportWorldType(EWorldType::Type WorldType) const
{
	return
		WorldType == EWorldType::Game ||
		WorldType == EWorldType::PIE;
}

void UEnvironmentSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_INFO(LogEnvironment, TEXT("Initialized"));

	AssetManager = Cast<URAssetManager>(UAssetManager::GetIfInitialized());
}

void UEnvironmentSubsystem::OnWorldComponentsUpdated(UWorld& InWorld)
{
	Super::OnWorldComponentsUpdated(InWorld);
	LOG_INFO(LogEnvironment, TEXT("OnWorldComponentsUpdated"));

	AWorldConfigSettings* WorldSettings = Cast<AWorldConfigSettings>(InWorld.GetWorldSettings());
	if (!IsValid(WorldSettings))
	{
		LOG_ERROR(LogEnvironment, TEXT("EnvironmentWorldSettings is invalid"));
		return;
	}

	UEnvironmentAsset* EnvironmentAsset = Cast<UEnvironmentAsset>(WorldSettings->EnvironmentAsset);
	if (!IsValid(EnvironmentAsset))
	{
		LOG_ERROR(LogEnvironment, TEXT("EnvironmentAsset is invalid"));
		return;
	}

	EnvironmentTag = EnvironmentAsset->ActorTag;

	for (const TSubclassOf<UEnvironmentStackedController>& Controller : EnvironmentAsset->StackedControllers)
	{
		RegisterStackedController(Controller);
	}

	for (const TSubclassOf<UEnvironmentDiscreteController>& Controller : EnvironmentAsset->DiscreteControllers)
	{
		RegisterDiscreteController(Controller);
	}

	LoadDefaultProfiles(EnvironmentAsset->DefaultProfiles);
}

void UEnvironmentSubsystem::Deinitialize()
{
	if (IsValid(AssetManager))
	{
		AssetManager->UnloadPrimaryAssetsWithType(UEnvironmentProfileAsset::GetPrimaryAssetType());
	}
	AssetManager = nullptr;

	for(const TPair<EEnvironmentProfileType, TObjectPtr<UEnvironmentStackedController>>& Kv : StackedControllers)
	{
		UEnvironmentStackedController* Controller = Kv.Value.Get();
		if (!Controller) continue;

		Controller->CleanupController();
		Controller->MarkAsGarbage();
	}
	StackedControllers.Empty();

	for(TObjectPtr<UEnvironmentDiscreteController> Value : DiscreateControllers)
	{
		UEnvironmentDiscreteController* Controller = Value.Get();
		if (!IsValid(Controller)) continue;

		Controller->CleanupController();
		Controller->MarkAsGarbage();
	}
	DiscreateControllers.Empty();

	LOG_INFO(LogEnvironment, TEXT("Deinitialized"));
	Super::Deinitialize();
}

