// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Factories/Factory.h"

// Generated Headers
#include "QuestEdFactory.generated.h"


/*
 * 
 */
UCLASS()
class UQuestEdFactory : public UFactory
{

	GENERATED_BODY()

public:

	UQuestEdFactory(const FObjectInitializer& ObjectInitializer);

	// ~ UFactory
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext) override;
	virtual bool CanCreateNew() const override;
	// ~ End of UFactory

};

