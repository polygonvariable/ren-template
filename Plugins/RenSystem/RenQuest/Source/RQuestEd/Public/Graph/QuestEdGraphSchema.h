// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Graph/EventflowEdGraphSchema.h"

// Generated Headers
#include "QuestEdGraphSchema.generated.h"


/*
 *
 */
UCLASS()
class UQuestEdGraphSchema : public UEventflowEdGraphSchema
{

	GENERATED_BODY()

protected:

	// ~ UEventflowEdGraphSchema
	virtual TMap<FName, UClass*> GetRegisteredNodeClasses() const override;
	// ~ End of UEventflowEdGraphSchema

};

