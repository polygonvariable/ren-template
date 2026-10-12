// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Generated Headers
#include "ObjectiveGlobalTaskFragment.generated.h"

// Forward Declarations
class UQuestEngine;


/*
 *
 */
USTRUCT()
struct FObjectiveGlobalTaskFragment
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FName TaskName;

	virtual void Execute(UQuestEngine* Engine) {};
	virtual ~FObjectiveGlobalTaskFragment() = default;

};

