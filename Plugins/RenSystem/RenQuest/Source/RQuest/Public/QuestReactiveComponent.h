// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameplayTagContainer.h"

// Generated Headers
#include "QuestReactiveComponent.generated.h"

// Forward Declarations
class UQuestSubsystem;
class UEventflowEngine;


/**
 *
 */
UCLASS(MinimalAPI, meta = (BlueprintSpawnableComponent))
class UQuestReactiveComponent : public UActorComponent
{

	GENERATED_BODY()

public:

	UQuestReactiveComponent();

	// ~ UActorComponent
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// ~ End of UActorComponent

protected:

	UPROPERTY()
	TObjectPtr<UQuestSubsystem> Subsystem;

	UPROPERTY(EditAnywhere, meta = (AllowedTypes = "Quest"))
	FPrimaryAssetId QuestId;

	UPROPERTY(EditAnywhere, meta = (Categories = "GameContext"))
	FGameplayTagContainer ContextTags;

	// ~ UQuestReactiveComponent
	void HandleOnQuestChanged(FPrimaryAssetId AssetId, UEventflowEngine* Engine);
	// ~ End of UQuestReactiveComponent

};

