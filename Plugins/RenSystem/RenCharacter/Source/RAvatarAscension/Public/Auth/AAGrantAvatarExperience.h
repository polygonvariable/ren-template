// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Auth/AuthAction.h"
#include "Definition/AscensionData.h"

// Generated Headers
#include "AAGrantAvatarExperience.generated.h"

// Forward Declarations
class UAvatarAsset;
class UAvatarStorageManager;
class UAssetManager;
class UCoreDataAsset;
struct FStreamableHandle;


/**
 *
 */
UCLASS(NotBlueprintType)
class UAAGrantAvatarExperience : public UAuthAction
{

	GENERATED_BODY()

public:

	FName TargetSourceId = NAME_None;
	FName MaterialSourceId = NAME_None;

	FGuid MaterialId;

	FPrimaryAssetId TargetAssetId;
	FPrimaryAssetId MaterialAssetId;
	
protected:

	UPROPERTY()
	TObjectPtr<const UAvatarAsset> TargetAsset = nullptr;

	UPROPERTY()
	TObjectPtr<const UCoreDataAsset> MaterialAsset = nullptr;

	UPROPERTY()
	TObjectPtr<UAvatarStorageManager> StorageManager = nullptr;

	UPROPERTY()
	TObjectPtr<UAssetManager> AssetManager = nullptr;

	int MaterialQuantity = 0;
	int Points = 0;

	int ExperiencePerLevel = 0;
	int LevelPerRank = 0;
	int MaxLevel = 0;
	int MaxRank = 0;

	FAscensionData AscensionData;


	void Step_LoadAssets();
	void Step_HandleOnAssetLoaded();
	void Step_CheckItemAsset();
	void Step_CheckMaterialAsset(const FGuid& ExperienceCollectionId);
	void Step_LoadBreakdownAsset(const FPrimaryAssetId& AssetId, int Quantity);
	void Step_HandleOnBreakdownAssetLoaded(FPrimaryAssetId AssetId, int Quantity);
	void Step_RemoveMaterial();
	void Step_AddExperience();

	// ~ UAuthAction
	void OnStarted() override;
	void OnCompleted(bool bSuccess) override;
	void OnCleanup() override;
	// ~ End of UAuthAction

private:

	TSharedPtr<FStreamableHandle> _AssetHandle;
	TSharedPtr<FStreamableHandle> _BreakdownHandle;

};

