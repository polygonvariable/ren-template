// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Blueprint/UserWidget.h"

// Project Headers
#include "Definition/InteractItem.h"

// Generated Headers
#include "InteractUI.generated.h"

// Forward Declarations
class UListView;
class UInteractSubsystem;
class UInteractEntry;


/**
 *
 */
UCLASS(Abstract)
class UInteractUI : public UUserWidget
{

	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UListView> InteractList;

	UPROPERTY()
	TMap<FGuid, TObjectPtr<UInteractEntry>> InteractEntries;

	UPROPERTY()
	TObjectPtr<UInteractSubsystem> InteractSubsystem = nullptr;


	UFUNCTION(BlueprintCallable)
	void ScrollSelection(int Direction);

	void HandleInteractAdded(const FGuid& InteractId, const FInteractItem& InteractItem);
	void HandleInteractRemoved(const FGuid& InteractId);

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

private:

	UPROPERTY()
	TArray<TObjectPtr<UInteractEntry>> _InteractPool;

};

