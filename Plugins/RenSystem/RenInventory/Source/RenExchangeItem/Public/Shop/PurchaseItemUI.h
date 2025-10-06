// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenExchangeItem/Public/ExchangeItemUI.h"

// Generated Headers
#include "PurchaseItemUI.generated.h"

// Forward Declarations
class UPurchaseItemSubsystem;

struct FExchangeRule;



/**
 *
 */
UCLASS(Abstract)
class UPurchaseItemUI : public UExchangeItemUI
{

	GENERATED_BODY()

protected:

	TWeakObjectPtr<UPurchaseItemSubsystem> PurchaseItemSubsystem;


	// ~ UExchangeItemUI
	virtual int GetExchangeQuantity() const override;
	virtual UExchangeItemSubsystem* GetExchangeItemSubsystem() const;
	virtual const FExchangeRule* GetExchangeRule(UObject* Target) const;
	// ~ End of UExchangeItemUI

protected:

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};

