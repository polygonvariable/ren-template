// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Factories/Factory.h"

// Project Headers

// Generated Headers
#include "EventflowEdFactory.generated.h"

// Forward Declarations



UCLASS()
class UEventflowEdFactory : public UFactory
{

	GENERATED_BODY()

public:

	UEventflowEdFactory(const FObjectInitializer& ObjectInitializer);

	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	virtual bool CanCreateNew() const override;

};

