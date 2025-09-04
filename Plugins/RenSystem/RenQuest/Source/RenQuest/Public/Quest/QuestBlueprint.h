// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenEventflow/Public/EventflowBlueprint.h"

// Generated Headers
#include "QuestBlueprint.generated.h"

// Forward Declarations
class UEventflowNode;

class IQuestObjectiveInterface;



/**
 *
 * 
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class UQuestBlueprint : public UEventflowBlueprint
{

	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Meta = (DisplayName = "RegisterObjective"))
	bool BP_RegisterObjective(UObject* Object, UEventflowNode* Node);

	IQuestObjectiveInterface* RegisterObjective(UObject* Object, UEventflowNode* Node);

	UFUNCTION(BlueprintCallable)
	UEventflowNode* UnregisterObjective(UObject* Object);

public:

	// ~ IEventflowBlueprint
	virtual void UnregisterBlueprint() override;
	// ~ End of IEventflowBlueprint

private:

	TArray<TWeakObjectPtr<UObject>> ObjectiveTasks;

};

