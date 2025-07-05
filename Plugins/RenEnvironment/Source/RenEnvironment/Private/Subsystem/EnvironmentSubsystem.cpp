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




bool UEnvironmentSubsystem::CreateStackedController(const TEnumAsByte<EEnvironmentProfileType> ProfileType, TSubclassOf<UEnvironmentStackedController> ControllerClass)
{
	if (EnvironmentStackedControllers.Contains(ProfileType))
	{
		LOG_ERROR(LogTemp, "Environment Controller already exists");
		return false;
	}

	UEnvironmentStackedController* NewController = NewObject<UEnvironmentStackedController>(this, ControllerClass);
	if (!IsValid(NewController))
	{
		LOG_ERROR(LogTemp, "Failed to create Environment Controller");
		return false;
	}

	NewController->InitializeController();
	EnvironmentStackedControllers.Add(ProfileType, NewController);

	return true;
}

bool UEnvironmentSubsystem::CreateDiscreteController(TSubclassOf<UEnvironmentDiscreteController> ControllerClass)
{
	UEnvironmentDiscreteController* NewController = NewObject<UEnvironmentDiscreteController>(this, ControllerClass);
	if (!IsValid(NewController))
	{
		LOG_ERROR(LogTemp, "Failed to create Environment Discrete Controller");
		return false;
	}

	NewController->InitializeController();
	EnvironmentDiscreateControllers.Push(NewController);

	return true;
}



void UEnvironmentSubsystem::AddStackedProfile(UEnvironmentProfileAsset* ProfileAsset, int Priority)
{
	if (!IsValid(ProfileAsset))
	{
		PRINT_ERROR(LogTemp, 2.0f, TEXT("ProfileAsset is not valid"));
		return;
	}

	if (UEnvironmentStackedController* Controller = EnvironmentStackedControllers.FindRef(ProfileAsset->ProfileType))
	{
		Controller->AddItem(ProfileAsset, Priority);
	}
}

void UEnvironmentSubsystem::RemoveStackedProfile(TEnumAsByte<EEnvironmentProfileType> ProfileType, int Priority)
{
	if (UEnvironmentStackedController* Controller = EnvironmentStackedControllers.FindRef(ProfileType))
	{
		Controller->RemoveItem(Priority);
	}
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

void UEnvironmentSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	LOG_WARNING(LogTemp, TEXT("EnvironmentSubsystem OnWorldBeginPlay"));

	AEnvironmentWorldSettings* WorldSettings = Cast<AEnvironmentWorldSettings>(InWorld.GetWorldSettings());
	if (IsValid(WorldSettings))
	{
		UEnvironmentAsset* EnvironmentAsset = WorldSettings->EnvironmentAsset;
		if (IsValid(EnvironmentAsset))
		{
			for (auto& Kvp : EnvironmentAsset->StackedControllers)
			{
				CreateStackedController(Kvp.Key, Kvp.Value);
			}

			for (auto It = EnvironmentAsset->DiscreteControllers.CreateIterator(); It; ++It)
			{
				CreateDiscreteController(*It);
			}
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

	for(UEnvironmentDiscreteController* Controller : EnvironmentDiscreateControllers)
	{
		Controller->CleanupController();
		Controller->MarkAsGarbage();
	}
	EnvironmentDiscreateControllers.Empty();

	LOG_WARNING(LogTemp, TEXT("EnvironmentSubsystem deinitialized"));
	Super::Deinitialize();
}

