// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameplayTagContainer.h"

// Generated Headers
#include "GameplayContextAction.generated.h"


/*
 *
 */
USTRUCT()
struct CGAMEPLAYCONTEXT_API FGameplayContextAction
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, meta = (Categories = "GameContext"))
	FGameplayTagContainer ContextTags;

	virtual bool IsDataValid() const;
    virtual bool Execute(UWorld* World, UObject* Caller);
	virtual void Reset();
    
	virtual ~FGameplayContextAction() = default;
    
};

