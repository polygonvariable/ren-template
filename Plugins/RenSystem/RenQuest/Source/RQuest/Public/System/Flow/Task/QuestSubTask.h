// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Core/IQuestRuntimeDataProvider.h"
#include "Task/EventflowSubTask.h"

// Generated Headers
#include "QuestSubTask.generated.h"


/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UQuestSubTask : public UEventflowSubTask, public IQuestRuntimeDataProvider
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Objective", meta = (EditCondition = "bShowInUI==true", EditConditionHides))
	FText ObjectiveTitle;

	UPROPERTY(EditAnywhere, Category = "Objective", meta = (EditCondition = "bShowInUI==true", EditConditionHides))
	FText ObjectiveDescription;

	UPROPERTY(EditAnywhere, Category = "Objective")
	bool bShowInUI = false;

	// ~ UEventflowTask
	virtual void CopyFromAsset(const UEventflowTask* Template) override
	{
		Super::CopyFromAsset(Template);

		const UQuestSubTask* Task = Cast<UQuestSubTask>(Template);
		checkf(Task, TEXT("Quest task contains invalid template or doesnt match its type"));

		ObjectiveTitle = Task->ObjectiveTitle;
		ObjectiveDescription = Task->ObjectiveDescription;
		bShowInUI = Task->bShowInUI;
	}
	// ~ End of UEventflowTask

};

