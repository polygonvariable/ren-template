// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "SubsystemInterface.h"

// Generated Headers
#include "RenGameInstanceSubsystem.generated.h"

// Forward Declarations
class UGameMetadataSettings;


/**
 *
 */
UCLASS(Abstract, Blueprintable, DisplayName = "Ren Game Instance Subsystem")
class RENCORE_API URenGameInstanceSubsystem : public UGameInstanceSubsystem, public ISubsystemInterface
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
	virtual UWorld* GetWorld() const override;

};

