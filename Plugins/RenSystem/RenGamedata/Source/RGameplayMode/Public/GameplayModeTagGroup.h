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

	UPROPERTY(EditDefaultsOnly, meta = (Categories = "Gameplay"))
	FGameplayTagContainer ActivateTags;

	UPROPERTY(EditDefaultsOnly, meta = (Categories = "InputMode"))
	FGameplayTag InputModeTag;

};

