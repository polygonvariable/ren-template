// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Graph/EventflowEdGraphNode.h"

// Generated Headers
#include "QuestEdGraphNode.generated.h"

// Forward Declarations
class UEventflowNodeData;
class UQuestPrimaryTask;
class UQuestSubTask;
class UQuestEngine;

class UQuestTask_Begin;
class UQuestTask_End;
class UQuestTask_ConditionalSpawnMarker;
class UQuestTask_SpawnMarker;
class UQuestTask_Reroute;
class UQuestTask_ExternalTask;


/*
 *
 */
UCLASS()
class UQuestEdNode_Base : public UEventflowEdGraphNode
{
	GENERATED_BODY()

public:

	UQuestEdNode_Base();

	FText Title;

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;


	virtual bool IsEntryNode() const override;

};


/*
 *
 */
UCLASS()
class UQuestEdNode_Reroute : public UQuestEdNode_Base
{

	GENERATED_BODY()

public:

	UQuestEdNode_Reroute();


	// ~ UEventflowEdGraphNode
	virtual UEventflowPrimaryTask* GetTask() const override;
	virtual void SetTask(UEventflowPrimaryTask* InTask) override;
	// ~ End of UEventflowEdGraphNode

	// ~ UEdGraphNode
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetNodeDescription() const override;
	virtual TArray<FText> GetRuntimeInputPins() const override;
	virtual TArray<FText> GetRuntimeOutputPins() const override;
	// ~ End of UEdGraphNode

protected:

	UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<UQuestTask_Reroute> Task = nullptr;

};



/*
 *
 */
UCLASS()
class UQuestEdNode_ExternalTask : public UQuestEdNode_Base
{

	GENERATED_BODY()

public:

	UQuestEdNode_ExternalTask();


	// ~ UEventflowEdGraphNode
	virtual UEventflowPrimaryTask* GetTask() const override;
	virtual void SetTask(UEventflowPrimaryTask* InTask) override;
	// ~ End of UEventflowEdGraphNode

	// ~ UEdGraphNode
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FText GetNodeDescription() const override;
	virtual void AllocateDefaultPins() override;
	virtual TArray<FText> GetRuntimeOutputPins() const override;
	// ~ End of UEdGraphNode

protected:

	UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<UQuestTask_ExternalTask> Task = nullptr;

};










/*
 *
 */
UCLASS()
class UQuestEdNode_Begin : public UQuestEdNode_Base
{

	GENERATED_BODY()

public:

	UQuestEdNode_Begin();

	// ~ UEventflowEdGraphNode
	virtual UEventflowPrimaryTask* GetTask() const override;
	virtual void SetTask(UEventflowPrimaryTask* InTask) override;
	virtual bool IsEntryNode() const override;
	// ~ End of UEventflowEdGraphNode

	// ~ UEdGraphNode
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FText GetNodeDescription() const override;
	virtual void AllocateDefaultPins() override;
	// ~ End of UEdGraphNode

protected:

	UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<UQuestTask_Begin> Task = nullptr;

};



/*
 *
 */
UCLASS()
class UQuestEdNode_End : public UQuestEdNode_Base
{

	GENERATED_BODY()

public:

	UQuestEdNode_End();

	// ~ UEventflowEdGraphNode
	virtual UEventflowPrimaryTask* GetTask() const override;
	virtual void SetTask(UEventflowPrimaryTask* InTask) override;
	// ~ End of UEventflowEdGraphNode

	// ~ UEdGraphNode
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FText GetNodeDescription() const override;
	virtual void AllocateDefaultPins() override;
	// ~ End of UEdGraphNode

protected:

	UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<UQuestTask_End> Task = nullptr;

};



/*
 *
 */
UCLASS()
class UQuestEdNode_SpawnMarker : public UQuestEdNode_Base
{

	GENERATED_BODY()

public:

	UQuestEdNode_SpawnMarker();

	// ~ UEventflowEdGraphNode
	virtual UEventflowPrimaryTask* GetTask() const override;
	virtual void SetTask(UEventflowPrimaryTask* InTask) override;
	// ~ End of UEventflowEdGraphNode

	// ~ UEdGraphNode
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FText GetNodeDescription() const override;
	virtual void AllocateDefaultPins() override;
	virtual TArray<FText> GetRuntimeOutputPins() const override;
	// ~ End of UEdGraphNode

protected:

	UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<UQuestTask_SpawnMarker> Task = nullptr;

};






/*
 *
 */
UCLASS()
class UQuestEdNode_ConditionalSpawnMarker : public UQuestEdNode_Base
{

	GENERATED_BODY()

public:

	UQuestEdNode_ConditionalSpawnMarker();

	// ~ UEventflowEdGraphNode
	virtual UEventflowPrimaryTask* GetTask() const override;
	virtual void SetTask(UEventflowPrimaryTask* InTask) override;
	// ~ End of UEventflowEdGraphNode

	// ~ UEdGraphNode
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FText GetNodeDescription() const override;
	virtual void AllocateDefaultPins() override;
	// ~ End of UEdGraphNode

protected:

	UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<UQuestTask_ConditionalSpawnMarker> Task = nullptr;

};


