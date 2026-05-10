// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Definition/AscensionData.h"
#include "Auth/AuthAction.h"

// Generated Headers
#include "Task_GrantItemExperience.generated.h"

// Forward Declarations
class UInventoryAsset;
class UInventoryStorageManager;
class URAssetManager;
class UCoreDataAsset;
struct FGameplayTagContainer;


/**
 *
 */
UCLASS(NotBlueprintType)
class UTask_GrantItemExperience : public UAuthAction
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
	TObjectPtr<UInventoryStorageManager> StorageManager = nullptr;

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

	// ~ UAuthAction
	void OnStarted() override;
	void OnCompleted(bool bSuccess) override;
	void OnCleanup() override;
	// ~ End of UAuthAction

};

