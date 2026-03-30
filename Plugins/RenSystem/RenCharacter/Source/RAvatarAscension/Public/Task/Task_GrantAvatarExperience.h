// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Definition/AscensionData.h"
#include "Task/TaskObject.h"

// Generated Headers
#include "Task_GrantAvatarExperience.generated.h"

// Forward Declarations
class UAvatarAsset;
class UAvatarStorage;
class URAssetManager;
class UCoreDataAsset;



/**
 *
 *
 *
 */
UCLASS(NotBlueprintType)
class UTask_GrantAvatarExperience : public UTaskObject
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
	TObjectPtr<UAvatarStorage> AvatarStorage = nullptr;

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

	// ~ UTaskObject
	void OnStarted() override;
	void OnStopped() override;
	void OnCleanup() override;
	// ~ End of UTaskObject

};

