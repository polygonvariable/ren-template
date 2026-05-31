// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Factories/Factory.h"

// Generated Headers
#include "DialogueEdFactory.generated.h"


/*
 * 
 */
UCLASS()
class UDialogueEdFactory : public UFactory
{

	GENERATED_BODY()

public:

	UDialogueEdFactory(const FObjectInitializer& ObjectInitializer);

	// ~ UFactory
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext) override;
	virtual bool CanCreateNew() const override;
	// ~ End of UFactory

};

