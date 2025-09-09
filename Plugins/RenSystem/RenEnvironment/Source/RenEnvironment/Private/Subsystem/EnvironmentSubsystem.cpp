// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Subsystem/EnvironmentSubsystem.h"

// Engine Header

// Project Header
#include "RCoreLibrary/Public/LogMacro.h"
#include "RenCore/Public/WorldConfigSettings.h"

#include "RenEnvironment/Public/Asset/EnvironmentAsset.h"
#include "RenEnvironment/Public/Asset/EnvironmentProfileAsset.h"
#include "RenEnvironment/Public/Controller/EnvironmentController.h"



bool UEnvironmentSubsystem::AddStackedProfile(UEnvironmentProfileAsset* ProfileAsset, int Priority)
{
	if (!IsValid(ProfileAsset))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("ProfileAsset is invalid"));
		return false;
	}
	
	UEnvironmentStackedController* Controller = EnvironmentStackedControllers.FindRef(ProfileAsset->ProfileType);
	if (!IsValid(Controller))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("EnvironmentStackedController is invalid or not found"));
		return false;
	}

	return Controller->AddItem(ProfileAsset, Priority);
}

bool UEnvironmentSubsystem::RemoveStackedProfile(UEnvironmentProfileAsset* ProfileAsset, int Priority)
{
	if (!IsValid(ProfileAsset))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("ProfileAsset is invalid"));
		return false;
	}

	UEnvironmentStackedController* Controller = EnvironmentStackedControllers.FindRef(ProfileAsset->ProfileType);
	if (!IsValid(Controller))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("EnvironmentStackedController is invalid or not found"));
		return false;
	}

	return Controller->RemoveItem(Priority);
}

void UEnvironmentSubsystem::LoadDefaultStackedProfiles(const TSet<TObjectPtr<UEnvironmentProfileAsset>>& ProfileAssets)
{
	for (auto& Profile : ProfileAssets)
	{
		AddStackedProfile(Profile, 0);
	}
}

bool UEnvironmentSubsystem::CreateStackedController(TSubclassOf<UEnvironmentStackedController> ControllerClass)
{
	if (!IsValid(ControllerClass))
	{
		LOG_ERROR(LogTemp, "ControllerClass is invalid");
		return false;
	}

	UEnvironmentStackedController* DefaultController = ControllerClass->GetDefaultObject<UEnvironmentStackedController>();
	if (EnvironmentStackedControllers.Contains(DefaultController->EnvironmentProfileType))
	{
		LOG_ERROR(LogTemp, "EnvironmentStackedController already exists");
		return false;
	}

	UEnvironmentStackedController* NewController = NewObject<UEnvironmentStackedController>(this, ControllerClass);
	if (!IsValid(NewController))
	{
		LOG_ERROR(LogTemp, "Failed to create EnvironmentStackedController");
		return false;
	}

	NewController->InitializeController();
	EnvironmentStackedControllers.Add(DefaultController->EnvironmentProfileType, NewController);

	return true;
}

bool UEnvironmentSubsystem::CreateDiscreteController(TSubclassOf<UEnvironmentDiscreteController> ControllerClass)
{
	if (!IsValid(ControllerClass))
	{
		LOG_ERROR(LogTemp, "ControllerClass is invalid");
		return false;
	}

	UEnvironmentDiscreteController* NewController = NewObject<UEnvironmentDiscreteController>(this, ControllerClass);
	if (!IsValid(NewController))
	{
		LOG_ERROR(LogTemp, "Failed to create EnvironmentDiscreteController");
		return false;
	}

	NewController->InitializeController();
	EnvironmentDiscreateControllers.Add(NewController);

	return true;
}

bool UEnvironmentSubsystem::DoesSupportWorldType(EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

void UEnvironmentSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogTemp, TEXT("EnvironmentSubsystem Initialized"));
}

void UEnvironmentSubsystem::OnWorldComponentsUpdated(UWorld& InWorld)
{
	Super::OnWorldComponentsUpdated(InWorld);
	LOG_WARNING(LogTemp, TEXT("EnvironmentSubsystem OnWorldComponentsUpdated"));

	AWorldConfigSettings* WorldSettings = Cast<AWorldConfigSettings>(InWorld.GetWorldSettings());
	if (!IsValid(WorldSettings))
	{
		LOG_ERROR(LogTemp, TEXT("EnvironmentWorldSettings is invalid"));
		return;
	}

	UEnvironmentAsset* EnvironmentAsset = Cast<UEnvironmentAsset>(WorldSettings->EnvironmentAsset);
	if (!IsValid(EnvironmentAsset))
	{
		LOG_ERROR(LogTemp, TEXT("EnvironmentAsset is invalid"));
		return;
	}

	for (const auto& Controller : EnvironmentAsset->StackedControllers)
	{
		CreateStackedController(Controller);
	}

	for (const auto& Controller : EnvironmentAsset->DiscreteControllers)
	{
		CreateDiscreteController(Controller);
	}

	LoadDefaultStackedProfiles(EnvironmentAsset->DefaultStackedProfiles);
}

void UEnvironmentSubsystem::Deinitialize()
{
	for(auto& Kvp : EnvironmentStackedControllers)
	{
		Kvp.Value->CleanupController();
		Kvp.Value->MarkAsGarbage();
	}
	EnvironmentStackedControllers.Empty();

	for(auto& Controller : EnvironmentDiscreateControllers)
	{
		Controller->CleanupController();
		Controller->MarkAsGarbage();
	}
	EnvironmentDiscreateControllers.Empty();

	LOG_WARNING(LogTemp, TEXT("EnvironmentSubsystem Deinitialized"));
	Super::Deinitialize();
}

