// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Graph/EventflowEdGraphSchema.h"

// Generated Headers
#include "DialogueEdGraphSchema.generated.h"


/*
 *
 */
UCLASS()
class UDialogueEdGraphSchema : public UEventflowEdGraphSchema
{

	GENERATED_BODY()

protected:

	// ~ UEventflowEdGraphSchema
	virtual TMap<FName, UClass*> GetRegisteredNodeClasses() const override;
	// ~ End of UEventflowEdGraphSchema

};

