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

	UPROPERTY(EditAnywhere, Category = "Objective", meta = (EditCondition = "bShowInUI==true", EditConditionHides))
	FText ObjectiveTitle;

	UPROPERTY(EditAnywhere, Category = "Objective", meta = (EditCondition = "bShowInUI==true", EditConditionHides))
	FText ObjectiveDescription;

	UPROPERTY(EditAnywhere, Category = "Objective")
	bool bCanBeCancelled = false;

	UPROPERTY(EditAnywhere, Category = "Objective")
	bool bShowInUI = false;

	// ~ UEventflowTask
	virtual void CopyFromAsset(const UEventflowTask* Template) override;
	// ~ End of UEventflowTask

protected:

	// ~ UEventflowTask
	virtual void OnInitialized(EFSMState PreviousState) override;
	virtual void OnLoaded(EFSMState PreviousState) override;
	virtual void OnReady(EFSMState PreviousState) override;
	virtual void OnActive(EFSMState PreviousState) override;
	// ~ End of UEventflowTask

};

