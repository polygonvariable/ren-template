// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/WorldSubsystem.h"

// Generated Headers
#include "SeasonSubsystem.generated.h"

// Module Macros
#define REN_API RSEASON_API

// Forward Declarations
class UAssetManager;
class USeasonController;
class USeasonWorldConfig;
struct FStreamableHandle;


/**
 *
 */
UCLASS()
class USeasonSubsystem : public UWorldSubsystem
{

	GENERATED_BODY()

public:

	REN_API USeasonController* GetSeasonController() const;

	// ~ UWorldSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void OnWorldComponentsUpdated(UWorld& InWorld) override;
	virtual void OnWorldEndPlay(UWorld& InWorld) override;
	// ~ End of UWorldSubsystem

protected:

	UPROPERTY()
	TObjectPtr<UAssetManager> AssetManager;

	UPROPERTY()
	TObjectPtr<const USeasonWorldConfig> SeasonConfig;

	UPROPERTY()
	TObjectPtr<USeasonController> SeasonController;

	TSharedPtr<FStreamableHandle> AssetHandle;


	bool CreateSeasonController(UClass* ControllerClass, UMaterialParameterCollection* MPC);
	void RemoveSeasonController();

	// ~ Binding
	void HandleOnSeasonLoaded();
	// ~ End of Binding

	// ~ UWorldSubsystem
	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;
	// ~ End of UWorldSubsystem

public:

	static REN_API USeasonSubsystem* Get(UWorld* World);

};


// Module Macros
#undef REN_API

