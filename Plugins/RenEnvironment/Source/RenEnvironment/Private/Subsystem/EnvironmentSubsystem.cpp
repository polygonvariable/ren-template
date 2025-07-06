// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Subsystem/EnvironmentSubsystem.h"

// Engine Header

// Project Header
#include "RenCore/Public/Macro/LogMacro.h"

#include "Asset/EnvironmentAsset.h"
#include "Asset/EnvironmentProfileAsset.h"
#include "Controller/EnvironmentController.h"
#include "EnvironmentWorldSettings.h"



void UEnvironmentSubsystem::AddStackedProfile(UEnvironmentProfileAsset* ProfileAsset, int Priority)
{
	if (!IsValid(ProfileAsset))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("ProfileAsset is not valid"));
		return;
	}

	UEnvironmentStackedController* Controller = EnvironmentStackedControllers.FindRef(ProfileAsset->ProfileType);
	if (IsValid(Controller))
	{
		Controller->AddItem(ProfileAsset, Priority);
	}
}

void UEnvironmentSubsystem::RemoveStackedProfile(UEnvironmentProfileAsset* ProfileAsset, int Priority)
{
	if (!IsValid(ProfileAsset))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("ProfileAsset is not valid"));
		return;
	}

	UEnvironmentStackedController* Controller = EnvironmentStackedControllers.FindRef(ProfileAsset->ProfileType);
	if (IsValid(Controller))
	{
		Controller->RemoveItem(Priority);
	}
}

void UEnvironmentSubsystem::LoadDefaultStackedProfiles()
{
	if (!IsValid(EnvironmentAsset))
	{
		LOG_ERROR(LogTemp, "EnvironmentAsset is not valid");
		return;
	}

	TSet<TObjectPtr<UEnvironmentProfileAsset>>& StackedProfiles = EnvironmentAsset->DefaultStackedProfiles;
	for (auto& Profile : StackedProfiles)
	{
		AddStackedProfile(Profile, 0);
	}
}

bool UEnvironmentSubsystem::CreateStackedController(TSubclassOf<UEnvironmentStackedController> ControllerClass)
{
	if (!IsValid(ControllerClass))
	{
		LOG_ERROR(LogTemp, "ControllerClass is not valid");
		return false;
	}

	UEnvironmentStackedController* EnvironmentCDO = ControllerClass->GetDefaultObject<UEnvironmentStackedController>();
	if (EnvironmentStackedControllers.Contains(EnvironmentCDO->EnvironmentProfileType))
	{
		LOG_ERROR(LogTemp, "Environment Controller already exists");
		return false;
	}

	UEnvironmentStackedController* NewController = NewObject<UEnvironmentStackedController>(this, ControllerClass);
	if (!IsValid(NewController))
	{
		LOG_ERROR(LogTemp, "Failed to create Environment Stacked Controller");
		return false;
	}

	NewController->InitializeController();
	EnvironmentStackedControllers.Add(EnvironmentCDO->EnvironmentProfileType, NewController);

	return true;
}

bool UEnvironmentSubsystem::CreateDiscreteController(TSubclassOf<UEnvironmentDiscreteController> ControllerClass)
{
	if (!IsValid(ControllerClass))
	{
		LOG_ERROR(LogTemp, "ControllerClass is not valid");
		return false;
	}

	UEnvironmentDiscreteController* NewController = NewObject<UEnvironmentDiscreteController>(this, ControllerClass);
	if (!IsValid(NewController))
	{
		LOG_ERROR(LogTemp, "Failed to create Environment Discrete Controller");
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
	LOG_WARNING(LogTemp, TEXT("EnvironmentSubsystem initialized"));
}

void UEnvironmentSubsystem::OnWorldComponentsUpdated(UWorld& InWorld)
{
	Super::OnWorldComponentsUpdated(InWorld);
	LOG_WARNING(LogTemp, TEXT("EnvironmentSubsystem OnWorldComponentsUpdated"));

	AEnvironmentWorldSettings* WorldSettings = Cast<AEnvironmentWorldSettings>(InWorld.GetWorldSettings());
	if (IsValid(WorldSettings))
	{
		EnvironmentAsset = WorldSettings->EnvironmentAsset;
		if (IsValid(EnvironmentAsset))
		{
			for (auto& Controller : EnvironmentAsset->StackedControllers)
			{
				CreateStackedController(Controller);
			}

			for (auto& Controller : EnvironmentAsset->DiscreteControllers)
			{
				CreateDiscreteController(Controller);
			}

			LoadDefaultStackedProfiles();
		}
		else
		{
			LOG_ERROR(LogTemp, TEXT("EnvironmentAsset is not valid"));
		}
	}
	else
	{
		LOG_ERROR(LogTemp, TEXT("EnvironmentWorldSettings is not valid"));
	}
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

	LOG_WARNING(LogTemp, TEXT("EnvironmentSubsystem deinitialized"));
	Super::Deinitialize();
}

