// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"

// Generated Headers
#include "InteractEntryUI.generated.h"

// Forward Declarations
class UTextBlock;


/**
 *
 */
UCLASS(Abstract)
class UInteractEntryUI : public UUserWidget, public IUserObjectListEntry
{

	GENERATED_BODY()

public:

	void UpdateDetails();

protected:

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> InteractTitle;


	// ~ IUserObjectListEntry
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnItemSelectionChanged(bool bSelected) override;
	// ~ End of IUserObjectListEntry

};

