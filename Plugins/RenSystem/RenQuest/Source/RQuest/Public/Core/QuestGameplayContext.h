// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "GameplayContextAction.h"

// Generated Headers
#include "QuestGameplayContext.generated.h"


/*
 *
 */
USTRUCT(DisplayName = "Start Quest")
struct FGameplayContext_StartQuest : public FGameplayContextAction
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, meta = (AllowedTypes = "Quest"))
	FPrimaryAssetId AssetId;

	// ~ FGameplayContextAction
	virtual bool Execute(UWorld* World, UObject* Owner, UObject* Instigator) override;
	// ~ End of FGameplayContextAction

};


/*
 *
 */
USTRUCT(DisplayName = "Quest Interact Handle")
struct FGameplayContext_QuestInteractionHandle : public FGameplayContextAction
{

	GENERATED_BODY()

public:

	// ~ FGameplayContextAction
	virtual bool Execute(UWorld* World, UObject* Owner, UObject* Instigator) override;
	// ~ End of FGameplayContextAction

};

