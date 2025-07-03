// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

// Project Headers
#include "SubsystemInterface.h"

// Generated Headers
#include "RenWorldSubsystem.generated.h"


/**
 * 
 */
UCLASS(Abstract, Blueprintable, DisplayName = "Ren World Subsystem")
class RENCORE_API URenWorldSubsystem : public UTickableWorldSubsystem, public ISubsystemInterface
{

	GENERATED_BODY()
	
public:

	virtual void PostInitialize_Implementation();

protected:

	virtual void OnInitialized_Implementation();
	virtual void OnDeinitialized_Implementation();

protected:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Object) const override;

};

