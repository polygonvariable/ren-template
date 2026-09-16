// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/EnvironmentSubsystem.h"

// Engine Headers
#include "Engine/AssetManager.h"
#include "EngineUtils.h"

// Project Headers
#include "Actor/EnvironmentActor.h"
#include "Core/AssetManagerUtil.h"
#include "Core/EnvironmentSettings.h"
#include "Data/EnvironmentWorldConfig.h"
#include "Data/EnvironmentProfileAsset.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "WorldFragmentSettings.h"
#include "System/EnvironmentController.h"
#include "Data/EnvironmentWorldConfig.h"


bool UEnvironmentSubsystem::AddProfile(UEnvironmentProfileAsset* ProfileAsset, int Priority)
{
	if (!IsValid(ProfileAsset))
	{
		LOG_ERROR(LogEnvironment, TEXT("ProfileAsset is invalid"));
		return false;
	}
	
	const EEnvironmentProfileType ProfileType = ProfileAsset->ProfileType;
	const TObjectPtr<UEnvironmentStackedController>* FoundController = StackedControllers.FindByPredicate([ProfileType](TObjectPtr<UEnvironmentStackedController> Item) { return IsValid(Item) && Item->ProfileType == ProfileType; });
	if (!FoundController)
	{
		LOG_ERROR(LogEnvironment, TEXT("Controller not found"));
		return false;
	}

	UEnvironmentStackedController* Controller = FoundController->Get();
	if (!IsValid(Controller))
	{
		LOG_ERROR(LogEnvironment, TEXT("EnvironmentStackedController is invalid"));
		return false;
	}

	return Controller->AddProfile(ProfileAsset, Priority);
}

bool UEnvironmentSubsystem::RemoveProfile(UEnvironmentProfileAsset* ProfileAsset, int Priority)
{
	if (!IsValid(ProfileAsset))
	{
		LOG_ERROR(LogEnvironment, TEXT("ProfileAsset is invalid"));
		return false;
	}
	return RemoveProfile(ProfileAsset->ProfileType, Priority);
}

bool UEnvironmentSubsystem::RemoveProfile(EEnvironmentProfileType ProfileType, int Priority)
{
	if (ProfileType == EEnvironmentProfileType::Default)
	{
		LOG_ERROR(LogEnvironment, TEXT("ProfileType is invalid"));
		return false;
	}

	const TObjectPtr<UEnvironmentStackedController>* FoundController = StackedControllers.FindByPredicate([ProfileType](TObjectPtr<UEnvironmentStackedController> Item) { return IsValid(Item) && Item->ProfileType == ProfileType; });
	if (!FoundController)
	{
		LOG_ERROR(LogEnvironment, TEXT("Controller not found"));
		return false;
	}

	UEnvironmentStackedController* Controller = FoundController->Get();
	if (!IsValid(Controller))
	{
		LOG_ERROR(LogEnvironment, TEXT("EnvironmentStackedController is invalid"));
		return false;
	}

	return Controller->RemoveProfile(Priority);
}


bool UEnvironmentSubsystem::RegisterStackedController(TSubclassOf<UEnvironmentStackedController> ControllerClass, AActor* EnvironmentActor)
{
	if (!IsValid(ControllerClass))
	{
		LOG_ERROR(LogEnvironment, TEXT("ControllerClass is invalid"));
		return false;
	}

	UEnvironmentStackedController* DefaultObject = ControllerClass->GetDefaultObject<UEnvironmentStackedController>();
	EEnvironmentProfileType ProfileType = DefaultObject->ProfileType;

	if (StackedControllers.ContainsByPredicate([ProfileType](TObjectPtr<UEnvironmentStackedController> Item) { return IsValid(Item) && Item->ProfileType == ProfileType; }))
	{
		LOG_ERROR(LogEnvironment, TEXT("Controller already exists"));
		return false;
	}

	UEnvironmentStackedController* Controller = NewObject<UEnvironmentStackedController>(this, ControllerClass);
	if (!IsValid(Controller))
	{
		LOG_ERROR(LogEnvironment, TEXT("EnvironmentStackedController is invalid"));
		return false;
	}

	Controller->Initialize(EnvironmentActor);
	StackedControllers.Add(Controller);

	return true;
}

bool UEnvironmentSubsystem::RegisterDiscreteController(TSubclassOf<UEnvironmentDiscreteController> ControllerClass, AActor* EnvironmentActor)
{
	if (!IsValid(ControllerClass))
	{
		LOG_ERROR(LogEnvironment, TEXT("ControllerClass is invalid"));
		return false;
	}

	UEnvironmentDiscreteController* Controller = NewObject<UEnvironmentDiscreteController>(this, ControllerClass);
	if (!IsValid(Controller))
	{
		LOG_ERROR(LogEnvironment, TEXT("EnvironmentDiscreteController is invalid"));
		return false;
	}

	Controller->Initialize(EnvironmentActor);
	DiscreateControllers.Add(Controller);

	return true;
}


void UEnvironmentSubsystem::RegisterControllers(const UEnvironmentWorldConfig* Data)
{
	AEnvironmentActor* Actor = nullptr;
	for (TActorIterator<AEnvironmentActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		Actor = *ActorItr;
		break;
	}

	for (const TSubclassOf<UEnvironmentStackedController>& Controller : Data->StackedControllers)
	{
		RegisterStackedController(Controller, Actor);
	}

	for (const TSubclassOf<UEnvironmentDiscreteController>& Controller : Data->DiscreteControllers)
	{
		RegisterDiscreteController(Controller, Actor);
	}
}

void UEnvironmentSubsystem::UnregisterControllers()
{
	for (TObjectPtr<UEnvironmentStackedController> Item : StackedControllers)
	{
		UEnvironmentStackedController* Controller = Item.Get();
		if (IsValid(Controller))
		{
			Controller->Deinitialize();
			Controller->MarkAsGarbage();
		}
	}
	StackedControllers.Empty();

	for (TObjectPtr<UEnvironmentDiscreteController> Item : DiscreateControllers)
	{
		UEnvironmentDiscreteController* Controller = Item.Get();
		if (IsValid(Controller))
		{
			Controller->Deinitialize();
			Controller->MarkAsGarbage();
		}
	}
	DiscreateControllers.Empty();
}


void UEnvironmentSubsystem::RegisterDefaultProfiles(const UEnvironmentWorldConfig* Data)
{
	const TArray<FPrimaryAssetId>& AssetIds = Data->DefaultProfiles;

	for (const FPrimaryAssetId& AssetId : AssetIds)
	{
		UEnvironmentProfileAsset* ProfileAsset = AssetManager->GetPrimaryAssetObject<UEnvironmentProfileAsset>(AssetId);
		if (IsValid(ProfileAsset))
		{
			AddProfile(ProfileAsset, Data->ProfilePriority);
		}
	}
}


void UEnvironmentSubsystem::HandleOnEnvironmentLoaded()
{
	FAssetManagerUtil::ReleaseHandle(ProfileHandle);

	if (!IsValid(WorldConfig))
	{
		LOG_ERROR(LogEnvironment, TEXT("EnvironmentFragment is invalid"));
		return;
	}

	RegisterControllers(WorldConfig);
	RegisterDefaultProfiles(WorldConfig);
}


bool UEnvironmentSubsystem::DoesSupportWorldType(EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

bool UEnvironmentSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!Super::ShouldCreateSubsystem(Outer) || GetClass() != UEnvironmentSettings::Get()->SubsystemClass)
	{
		return false;
	}

	const UEnvironmentWorldConfig* Config = AWorldFragmentSettings::GetConfigByClass<UEnvironmentWorldConfig>(Cast<UWorld>(Outer));
	if (!IsValid(Config))
	{
		return false;
	}

	return Config->bEnabled;
}

void UEnvironmentSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogEnvironment, TEXT("EnvironmentSubsystem Initialized"));

	AssetManager = UAssetManager::GetIfInitialized();
}

void UEnvironmentSubsystem::OnWorldComponentsUpdated(UWorld& InWorld)
{
	Super::OnWorldComponentsUpdated(InWorld);
	LOG_WARNING(LogEnvironment, TEXT("EnvironmentSubsystem OnWorldComponentsUpdated"));

	FAssetManagerUtil::CancelHandle(ProfileHandle);

	WorldConfig = AWorldFragmentSettings::GetConfigByClass<UEnvironmentWorldConfig>(&InWorld);
	if (!IsValid(WorldConfig))
	{
		LOG_ERROR(LogEnvironment, TEXT("EnvironmentFragment is invalid"));
		return;
	}
	
	const UEnvironmentSettings* Settings = UEnvironmentSettings::Get();

	const TArray<FName>& Bundles = Settings->EnvironmentBundles;
	const TArray<FPrimaryAssetId>& Profiles = WorldConfig->DefaultProfiles;

	ProfileHandle = AssetManager->LoadPrimaryAssets(Profiles, Bundles, FStreamableDelegate::CreateUObject(this, &UEnvironmentSubsystem::HandleOnEnvironmentLoaded));
}

void UEnvironmentSubsystem::Deinitialize()
{
	UnregisterControllers();

	FAssetManagerUtil::CancelHandle(ProfileHandle);
	if (IsValid(AssetManager))
	{
		AssetManager->UnloadPrimaryAssetsWithType(UEnvironmentProfileAsset::GetPrimaryAssetType());
	}
	AssetManager = nullptr;

	LOG_WARNING(LogEnvironment, TEXT("EnvironmentSubsystem Deinitialized"));
	Super::Deinitialize();
}


UEnvironmentSubsystem* UEnvironmentSubsystem::Get(UWorld* World)
{
	if (!IsValid(World))
	{
		return nullptr;
	}
	return World->GetSubsystem<UEnvironmentSubsystem>();
}

