// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

// Project Headers
#include "RenEnvironment/Public/Profile/EnvironmentProfileType.h"

// Generated Headers
#include "EnvironmentSubsystem.generated.h"

// Forward Declarations
class UEnvironmentDiscreteController;
class UEnvironmentStackedController;
class UEnvironmentProfileAsset;
class UEnvironmentAsset;


/**
 *
 */
UCLASS(Blueprintable) // Add Blueprintable for easiy debug in live blueprint debugger
class RENENVIRONMENT_API UEnvironmentSubsystem : public UWorldSubsystem
{

	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void AddStackedProfile(UEnvironmentProfileAsset* ProfileAsset, int Priority);

	UFUNCTION(BlueprintCallable)
	void RemoveStackedProfile(UEnvironmentProfileAsset* ProfileAsset, int Priority);

protected:

	UPROPERTY(BlueprintReadOnly)
	TMap<TEnumAsByte<EEnvironmentProfileType>, TObjectPtr<UEnvironmentStackedController>> EnvironmentStackedControllers;

	UPROPERTY(BlueprintReadOnly)
	TSet<TObjectPtr<UEnvironmentDiscreteController>> EnvironmentDiscreateControllers;

	UPROPERTY()
	TObjectPtr<UEnvironmentAsset> EnvironmentAsset;


	void LoadDefaultStackedProfiles();

	bool CreateStackedController(TSubclassOf<UEnvironmentStackedController> ControllerClass);
	bool CreateDiscreteController(TSubclassOf<UEnvironmentDiscreteController> ControllerClass);

protected:

	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void OnWorldComponentsUpdated(UWorld& InWorld) override;
	virtual void Deinitialize() override;

};

