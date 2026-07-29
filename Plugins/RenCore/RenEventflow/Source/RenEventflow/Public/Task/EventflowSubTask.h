// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "EventflowTask.h"

// Generated Headers
#include "EventflowSubTask.generated.h"

// Module Macros
#define REN_API RENEVENTFLOW_API


/**
 *
 */
UCLASS(MinimalAPI)
class UEventflowSubTask : public UEventflowTask
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FName TaskName;

	// ~ UEventflowTask
	REN_API virtual void CopyFromAsset(const UEventflowTask* Template) override;
	// ~ End of UEventflowTask

};


// Module Macros
#undef REN_API

