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
class URAssetManager;

class UEnvironmentDiscreteController;
class UEnvironmentStackedController;
class UEnvironmentProfileAsset;



/**
 *
 */
UCLASS(MinimalAPI)
class UEnvironmentSubsystem : public UWorldSubsystem
{

	GENERATED_BODY()

public:

	virtual AActor* GetEnvironmentActor();

	RENENVIRONMENT_API bool AddProfile(UEnvironmentProfileAsset* ProfileAsset, int Priority);
	RENENVIRONMENT_API bool RemoveProfile(UEnvironmentProfileAsset* ProfileAsset, int Priority);

	RENENVIRONMENT_API void AddProfile(const FGuid& LatentId, const FPrimaryAssetId& AssetId, int Priority);
	RENENVIRONMENT_API bool RemoveProfile(const FGuid& LatentId, const FPrimaryAssetId& AssetId, int Priority);

	RENENVIRONMENT_API void AddProfile(const FPrimaryAssetId& AssetId, int Priority);
	RENENVIRONMENT_API bool RemoveProfile(const FPrimaryAssetId& AssetId, int Priority);

protected:


	FName EnvironmentTag = TEXT("Actor.Environment");

	TArray<TPair<FPrimaryAssetId, int>> CancelledProfiles;

	UPROPERTY()
	TObjectPtr<URAssetManager> AssetManager;

	UPROPERTY()
	TWeakObjectPtr<AActor> EnvironmentActor;

	UPROPERTY()
	TMap<EEnvironmentProfileType, TObjectPtr<UEnvironmentStackedController>> StackedControllers;

	UPROPERTY()
	TArray<TObjectPtr<UEnvironmentDiscreteController>> DiscreateControllers;


	bool RemoveProfile(EEnvironmentProfileType ProfileType, int Priority);

	bool RegisterStackedController(TSubclassOf<UEnvironmentStackedController> ControllerClass);
	bool RegisterDiscreteController(TSubclassOf<UEnvironmentDiscreteController> ControllerClass);

	void LoadDefaultProfiles(const TArray<FPrimaryAssetId>& AssetIds);

protected:

	// ~ UWorldSubsystem
	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void OnWorldComponentsUpdated(UWorld& InWorld) override;
	virtual void Deinitialize() override;
	// ~ End of UWorldSubsystem

};
