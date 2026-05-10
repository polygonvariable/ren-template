// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Definition/AscensionData.h"
#include "Auth/AuthAction.h"

// Generated Headers
#include "Task_GrantAvatarExperience.generated.h"

// Forward Declarations
class UAvatarAsset;
class UAvatarStorageManager;
class URAssetManager;
class UCoreDataAsset;


/**
 *
 */
UCLASS(NotBlueprintType)
class UTask_GrantAvatarExperience : public UAuthAction
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
	TObjectPtr<URAssetManager> AssetManager = nullptr;

	int MaterialQuantity = 0;
	int Points = 0;

	int ExperiencePerLevel = 0;
	int LevelPerRank = 0;
	int MaxLevel = 0;
	int MaxRank = 0;

	FAscensionData AscensionData;


	void Step_LoadAssets();
	void Step_CheckItemAsset();
	void Step_CheckMaterialAsset(const FGuid& ExperienceCollectionId);
	void Step_LoadBreakdownAsset(const FPrimaryAssetId& AssetId, int Quantity);
	void Step_RemoveMaterial();
	void Step_AddExperience();

	// ~ UAuthAction
	void OnStarted() override;
	void OnCompleted(bool bSuccess) override;
	void OnCleanup() override;
	// ~ End of UAuthAction

};

