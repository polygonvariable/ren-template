// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Auth/AuthAction.h"

// Generated Headers
#include "AAGrantItemRank.generated.h"

// Forward Declarations
class UInventoryAsset;
class UInventoryStorageManager;
class URAssetManager;
struct FStreamableHandle;


/**
 *
 */
UCLASS(NotBlueprintType)
class UAAGrantItemRank : public UAuthAction
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
	TObjectPtr<UAssetManager> AssetManager = nullptr;


	void Step_LoadAsset();
	void Step_HandleOnAssetLoaded();
	void Step_CheckTarget();

	// ~ UAuthAction
	void OnStarted() override;
	void OnCompleted(bool bSuccess) override;
	void OnCleanup() override;
	// ~ End of UAuthAction

private:

	TSharedPtr<FStreamableHandle> _AssetHandle = nullptr;

};

