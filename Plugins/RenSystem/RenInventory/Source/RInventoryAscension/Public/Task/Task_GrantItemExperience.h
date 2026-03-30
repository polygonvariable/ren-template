// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Definition/AscensionData.h"
#include "Task/TaskObject.h"

// Generated Headers
#include "Task_GrantItemExperience.generated.h"

// Forward Declarations
class UInventoryAsset;
class UInventoryStorage;
class URAssetManager;
class UCoreDataAsset;
struct FGameplayTagContainer;



/**
 *
 *
 *
 */
UCLASS(NotBlueprintType)
class UTask_GrantItemExperience : public UTaskObject
{

	GENERATED_BODY()

public:

	FName SourceId = NAME_None;
	FGuid TargetId;
	FGuid MaterialId;

	FPrimaryAssetId TargetAssetId;
	FPrimaryAssetId MaterialAssetId;
	
protected:

	UPROPERTY()
	TObjectPtr<const UInventoryAsset> TargetAsset = nullptr;

	UPROPERTY()
	TObjectPtr<const UCoreDataAsset> MaterialAsset = nullptr;

	UPROPERTY()
	TObjectPtr<UInventoryStorage> Inventory = nullptr;

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
	void Step_CheckMaterialAsset(const FGuid& MaterialCollectionId);
	void Step_LoadBreakdownAsset(const FPrimaryAssetId& AssetId, int Quantity);
	void Step_RemoveItem();
	void Step_AddExperience();

	// ~ UTaskObject
	void OnStarted() override;
	void OnStopped() override;
	void OnCleanup() override;
	// ~ End of UTaskObject

};

