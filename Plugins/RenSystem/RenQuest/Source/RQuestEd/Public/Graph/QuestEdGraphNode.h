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
class UQuestTask_SubtaskGate;
class UQuestTask_EnsureGlobalTask;
class UQuestTask_CheckStorage;
class UQuestTask_WidgetGate;

/*
 *
 */
UCLASS()
class UQuestEdGraphNode : public UEventflowEdGraphNode
{

	GENERATED_BODY()

public:

	UQuestEdGraphNode();

};


/*
 *
 */
UCLASS(meta = (QuestNode))
class UQuestEdNode_Reroute : public UQuestEdGraphNode
{

	GENERATED_BODY()

public:

	UQuestEdNode_Reroute();

	// ~ UEventflowEdGraphNode
	virtual TSubclassOf<UEventflowNodeTask> GetTaskClass() const;
	// ~ UEventflowEdGraphNode

	// ~ UEdGraphNode
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual TArray<FText> GetRuntimeInputPins() const override;
	virtual TArray<FText> GetRuntimeOutputPins() const override;
	// ~ End of UEdGraphNode

};



/*
 *
 */
UCLASS(meta = (QuestNode))
class UQuestEdNode_ExternalTask : public UQuestEdGraphNode
{

	GENERATED_BODY()

public:

	UQuestEdNode_ExternalTask();

	// ~ UEventflowEdGraphNode
	virtual TSubclassOf<UEventflowNodeTask> GetTaskClass() const;
	// ~ UEventflowEdGraphNode
	
	// ~ UEdGraphNode
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual void AllocateDefaultPins() override;
	virtual void SyncRuntimeData() override;
	virtual TArray<FText> GetRuntimeOutputPins() const override;
	// ~ End of UEdGraphNode

};



/*
 *
 */
UCLASS(meta = (QuestNode))
class UQuestEdNode_CheckStorage : public UQuestEdGraphNode
{

	GENERATED_BODY()

public:

	UQuestEdNode_CheckStorage();

	// ~ UEventflowEdGraphNode
	virtual TSubclassOf<UEventflowNodeTask> GetTaskClass() const;
	// ~ UEventflowEdGraphNode

	// ~ UEdGraphNode
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual void AllocateDefaultPins() override;
	// ~ End of UEdGraphNode

};




/*
 *
 */
UCLASS(meta = (QuestNode))
class UQuestEdNode_SubtaskGate : public UQuestEdGraphNode
{

	GENERATED_BODY()

public:

	UQuestEdNode_SubtaskGate();

	// ~ UEventflowEdGraphNode
	virtual TSubclassOf<UEventflowNodeTask> GetTaskClass() const;
	// ~ UEventflowEdGraphNode

	// ~ UEdGraphNode
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual void AllocateDefaultPins() override;
	// ~ End of UEdGraphNode

};


/*
 *
 */
UCLASS(meta = (QuestNode))
class UQuestEdNode_WidgetGate : public UQuestEdGraphNode
{

	GENERATED_BODY()

public:

	UQuestEdNode_WidgetGate();

	// ~ UEventflowEdGraphNode
	virtual TSubclassOf<UEventflowNodeTask> GetTaskClass() const;
	// ~ UEventflowEdGraphNode

	// ~ UEdGraphNode
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual void AllocateDefaultPins() override;
	// ~ End of UEdGraphNode

};



/*
 *
 */
UCLASS(meta = (QuestNode))
class UQuestEdNode_EnsureGlobal : public UQuestEdGraphNode
{

	GENERATED_BODY()

public:

	UQuestEdNode_EnsureGlobal();

	// ~ UEventflowEdGraphNode
	virtual TSubclassOf<UEventflowNodeTask> GetTaskClass() const;
	// ~ UEventflowEdGraphNode

	// ~ UEdGraphNode
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual void AllocateDefaultPins() override;
	// ~ End of UEdGraphNode

};





/*
 *
 */
UCLASS(meta = (QuestNode))
class UQuestEdNode_Begin : public UQuestEdGraphNode
{

	GENERATED_BODY()

public:

	UQuestEdNode_Begin();

	// ~ UEventflowEdGraphNode
	virtual TSubclassOf<UEventflowNodeTask> GetTaskClass() const;
	// ~ UEventflowEdGraphNode
	
	// ~ UEdGraphNode
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual void AllocateDefaultPins() override;
	// ~ End of UEdGraphNode

};



/*
 *
 */
UCLASS(meta = (QuestNode))
class UQuestEdNode_End : public UQuestEdGraphNode
{

	GENERATED_BODY()

public:

	UQuestEdNode_End();

	// ~ UEventflowEdGraphNode
	virtual TSubclassOf<UEventflowNodeTask> GetTaskClass() const;
	// ~ UEventflowEdGraphNode
	
	// ~ UEdGraphNode
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual void AllocateDefaultPins() override;
	// ~ End of UEdGraphNode

};



/*
 *
 */
UCLASS(meta = (QuestNode))
class UQuestEdNode_SpawnMarker : public UQuestEdGraphNode
{

	GENERATED_BODY()

public:

	UQuestEdNode_SpawnMarker();

	// ~ UEventflowEdGraphNode
	virtual TSubclassOf<UEventflowNodeTask> GetTaskClass() const;
	// ~ UEventflowEdGraphNode
	
	// ~ UEdGraphNode
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual void AllocateDefaultPins() override;
	virtual TArray<FText> GetRuntimeOutputPins() const override;
	// ~ End of UEdGraphNode

};






/*
 *
 */
UCLASS(meta = (QuestNode))
class UQuestEdNode_ConditionalSpawnMarker : public UQuestEdGraphNode
{

	GENERATED_BODY()

public:

	UQuestEdNode_ConditionalSpawnMarker();

	// ~ UEventflowEdGraphNode
	virtual TSubclassOf<UEventflowNodeTask> GetTaskClass() const;
	// ~ UEventflowEdGraphNode
	
	// ~ UEdGraphNode
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual void AllocateDefaultPins() override;
	// ~ End of UEdGraphNode

};


