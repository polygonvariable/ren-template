// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenExchangeItem/Public/ExchangeItemSubsystem.h"

// Generated Headers
#include "CraftItemSubsystem.generated.h"

// Forward Declarations
struct FExchangeRule;



/**
 *
 */
UCLASS()
class UCraftItemSubsystem : public UExchangeItemSubsystem
{

	GENERATED_BODY()

protected:

	// ~ UExchangeItemSubsystem
	virtual const FExchangeRule* GetExchangeRule(UObject* Target) const override;
	// ~ End of UExchangeItemSubsystem

public:

	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem

};

