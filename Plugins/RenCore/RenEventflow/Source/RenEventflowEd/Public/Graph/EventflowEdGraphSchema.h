// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Generated Headers
#include "EventflowEdGraphSchema.generated.h"

// Module Macros
#define REN_API RENEVENTFLOWED_API


/*
 * 
 */
UCLASS()
class REN_API UEventflowEdGraphSchema : public UEdGraphSchema
{

	GENERATED_BODY()

public:

	// ~ UEdGraphSchema
	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;
	virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override;
	// ~ End of UEdGraphSchema

	UClass* GetRegisteredNodeClass(const FName& ClassName) const;

protected:

	virtual TMap<FName, UClass*> GetRegisteredNodeClasses() const;

public:

	static const FName PC_Exec;
	static const FName PC_Wildcard;

};


// Module Macros
#undef REN_API

