// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Definition/AscensionData.h"
#include "Auth/AuthAction.h"

// Generated Headers
#include "Task_GrantItemRank.generated.h"

// Forward Declarations
class UInventoryAsset;
class UInventoryStorageManager;
class URAssetManager;


/**
 *
 */
UCLASS(NotBlueprintType)
class UTask_GrantItemRank : public UAuthAction
{

	GENERATED_BODY()

public:

	FName SourceId = NAME_None;
	FGuid TargetId;
	FPrimaryAssetId TargetAssetId;
	
protected:

	UPROPERTY()
	TObjectPtr<const UInventoryAsset> TargetAsset = nullptr;

	UPROPERTY()
	TObjectPtr<UInventoryStorageManager> StorageManager = nullptr;

	UPROPERTY()
	TObjectPtr<URAssetManager> AssetManager = nullptr;

	int ExperiencePerLevel = 0;
	int LevelPerRank = 0;
	int MaxLevel = 0;
	int MaxRank = 0;

	FAscensionData AscensionData;


	void Step_LoadAsset();
	void Step_CheckTarget();

	// ~ UAuthAction
	void OnStarted() override;
	void OnCompleted(bool bSuccess) override;
	void OnCleanup() override;
	// ~ End of UAuthAction

};

