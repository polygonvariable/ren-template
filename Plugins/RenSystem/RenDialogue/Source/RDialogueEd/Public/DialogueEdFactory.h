// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Factories/Factory.h"

// Project Headers

// Generated Headers
#include "DialogueEdFactory.generated.h"

// Forward Declarations



UCLASS()
class UDialogueEdFactory : public UFactory
{

	GENERATED_BODY()

public:

	UDialogueEdFactory(const FObjectInitializer& ObjectInitializer);

	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext) override;
	virtual bool CanCreateNew() const override;

};

