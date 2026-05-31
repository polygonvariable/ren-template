// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Factories/Factory.h"

// Generated Headers
#include "EventflowEdFactory.generated.h"


/*
 *
 */
UCLASS()
class UEventflowEdFactory : public UFactory
{

	GENERATED_BODY()

public:

	UEventflowEdFactory(const FObjectInitializer& ObjectInitializer);

	// ~ UFactory
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	virtual bool CanCreateNew() const override;
	// ~ End of UFactory

};

