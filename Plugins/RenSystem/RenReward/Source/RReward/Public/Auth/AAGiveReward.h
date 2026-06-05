// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Auth/AuthAction.h"

// Generated Headers
#include "AAGiveReward.generated.h"

// Forward Declarations
class UAssetManager;
class UAssetCollection;
class URewardSubsystem;
struct FStreamableHandle;


/**
 * Auth action to give a reward
 */
UCLASS(NotBlueprintType)
class UAAGiveReward : public UAuthAction
{

	GENERATED_BODY()

public:

	FPrimaryAssetId RewardAssetId;

protected:

	TObjectPtr<URewardSubsystem> RewardSubsystem = nullptr;
	TObjectPtr<UAssetManager> AssetManager = nullptr;


	void Step_LoadReward();
	void Step_OnRewardLoaded();
	void Step_AddReward(const UAssetCollection* Collection);
	void Step_Finalize();

	// ~ UAuthAction
	virtual void OnStarted() override;
	virtual void OnCleanup() override;
	// ~ End of UAuthAction

private:

	TSharedPtr<FStreamableHandle> RewardHandle = nullptr;

};

