// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"

// Generated Headers
#include "GameplayModeTagGroup.generated.h"


/*
 *
 */
USTRUCT()
struct FGameplayModeTagGroup : public FTableRowBase
{

	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Mode", meta = (Categories = "Gameplay"))
	FGameplayTagContainer ActivateTags;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Mode")
	bool bAllowPopWithTag;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Mode", meta = (Categories = "Gameplay"))
	FGameplayTag PopWithTag;

	UPROPERTY(EditDefaultsOnly, Category = "Input Mode", meta = (Categories = "InputMode"))
	FGameplayTag InputModeTag;

};

