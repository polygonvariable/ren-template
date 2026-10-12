// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Core/IQuestRuntimeDataProvider.h"
#include "Task/EventflowNodeTask.h"

// Generated Headers
#include "QuestPrimaryTask.generated.h"


/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UQuestPrimaryTask : public UEventflowNodeTask, public IQuestRuntimeDataProvider
{

	GENERATED_BODY()

public:

	UQuestPrimaryTask();

	UPROPERTY(EditAnywhere, Category = "Objective")
	FName Summary;

	UPROPERTY(EditAnywhere, Category = "Objective")
	bool bIsTransient = true;

	UPROPERTY(EditAnywhere, Category = "Objective")
	bool bCanCancel = false;

	// ~ UEventflowTask
	virtual void CopyFromAsset(const UEventflowTask* Template) override;
	// ~ End of UEventflowTask

};

