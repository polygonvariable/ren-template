// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/WorldSubsystem.h"

// Project Headers
#include "Core/Type/EnvironmentProfileType.h"

// Generated Headers
#include "EnvironmentSubsystem.generated.h"

// Module Macros
#define REN_API RENVIRONMENT_API

// Forward Declarations
class UAssetManager;
class UEnvironmentDiscreteController;
class UEnvironmentStackedController;
class UEnvironmentProfileAsset;
class UEnvironmentWorldConfig;
struct FStreamableHandle;


/**
 * 
 */
UCLASS(MinimalAPI)
class UEnvironmentSubsystem : public UWorldSubsystem
{

	GENERATED_BODY()

public:

	REN_API bool AddProfile(UEnvironmentProfileAsset* ProfileAsset, int Priority);
	REN_API bool RemoveProfile(UEnvironmentProfileAsset* ProfileAsset, int Priority);
	REN_API bool RemoveProfile(EEnvironmentProfileType ProfileType, int Priority);

protected:

	UPROPERTY()
	TObjectPtr<UAssetManager> AssetManager;

	UPROPERTY()
	TArray<TObjectPtr<UEnvironmentDiscreteController>> DiscreateControllers;

	UPROPERTY()
	TArray<TObjectPtr<UEnvironmentStackedController>> StackedControllers;

	TSharedPtr<FStreamableHandle> ProfileHandle;


	bool RegisterStackedController(TSubclassOf<UEnvironmentStackedController> ControllerClass, AActor* EnvironmentActor);
	bool RegisterDiscreteController(TSubclassOf<UEnvironmentDiscreteController> ControllerClass, AActor* EnvironmentActor);

	void RegisterControllers(const UEnvironmentWorldConfig* Data);
	void UnregisterControllers();

	void RegisterDefaultProfiles(const UEnvironmentWorldConfig* Data);
	const UEnvironmentWorldConfig* GetEnvironmentWorldConfig() const;

	// ~ Binding
	void HandleOnEnvironmentLoaded();
	// ~ End of Binding

protected:

	// ~ UWorldSubsystem
	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void OnWorldComponentsUpdated(UWorld& InWorld) override;
	virtual void Deinitialize() override;
	// ~ End of UWorldSubsystem

public:

	static REN_API UEnvironmentSubsystem* Get(UWorld* World);

};


// Module Macros
#undef REN_API

