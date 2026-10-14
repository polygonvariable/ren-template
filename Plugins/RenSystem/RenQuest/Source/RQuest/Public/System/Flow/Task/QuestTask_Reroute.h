// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "System/Flow/Task/QuestPrimaryTask.h"

// Generated Headers
#include "QuestTask_Reroute.generated.h"


/**
 *
 */
UENUM()
enum class ERerouteType : uint8
{
	Source, /* That sends signal */
	Target, /* recieve the reroute signal */
};

/**
 *
 */
UCLASS(MinimalAPI)
class UQuestTask_Reroute : public UQuestPrimaryTask
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Reroute")
	ERerouteType RerouteType;

	UPROPERTY(EditAnywhere, Category = "Reroute")
	FString RerouteName;

	UPROPERTY(VisibleAnywhere, Category = "Reroute", meta = (EditCondition = "RerouteType==ERerouteType::Source", EditConditionHides))
	FGuid RerouteId;


	// ~ UEventflowNodeTask
	virtual TInstancedStruct<FEventflowNodeTransitionData>& GetTransitionData(EFSMResult Result) override;
	// ~ End of UEventflowNodeTask

	// ~ UEventflowTask
	virtual void CopyFromAsset(const UEventflowTask* Template) override;
	// ~ End of UEventflowTask

	// ~ UObject
	virtual void PreSave(FObjectPreSaveContext ObjectSaveContext) override;
	// ~ End of UObject

};

