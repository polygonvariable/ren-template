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



/**
 *
 */
UCLASS(Blueprintable) // Add Blueprintable for easiy debug in live blueprint debugger
class UEnvironmentSubsystem : public UWorldSubsystem
{

	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void AddStackedProfile(UEnvironmentProfileAsset* ProfileAsset, int Priority);

	UFUNCTION(BlueprintCallable)
	void RemoveStackedProfile(TEnumAsByte<EEnvironmentProfileType> ProfileType, int Priority);

protected:

	UPROPERTY(BlueprintReadOnly)
	TMap<TEnumAsByte<EEnvironmentProfileType>, TObjectPtr<UEnvironmentStackedController>> EnvironmentStackedControllers;

	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<UEnvironmentDiscreteController>> EnvironmentDiscreateControllers;


	bool CreateStackedController(const TEnumAsByte<EEnvironmentProfileType> ProfileType, TSubclassOf<UEnvironmentStackedController> ControllerClass);
	bool CreateDiscreteController(TSubclassOf<UEnvironmentDiscreteController> ControllerClass);

protected:

	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Deinitialize() override;

};

