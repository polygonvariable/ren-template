// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Blueprint/UserWidget.h"

// Project Headers
#include "InteractItem.h"

// Generated Headers
#include "InteractUI.generated.h"

// Forward Declarations
class UListView;
class UInputAction;
class UInteractSubsystem;
class UInteractEntry;
struct FInputActionValue;


/**
 *
 */
UCLASS(Abstract)
class UInteractUI : public UUserWidget
{

	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> InteractScroll = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> InteractSelect = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UListView> InteractList;

	UPROPERTY()
	TMap<FGuid, TObjectPtr<UInteractEntry>> InteractEntries;

	UPROPERTY()
	TObjectPtr<UInteractSubsystem> InteractSubsystem = nullptr;


	void ScrollSelection(int Direction);

	// ~ Binding
	void HandleInteractAdded(const FGuid& InteractId, const FInteractItem& InteractItem);
	void HandleInteractRemoved(const FGuid& InteractId);
	// ~ End of Binding

	// ~ Input Binding
	void HandleOnInteractScroll(const FInputActionValue& Value);
	void HandleOnInteractSelect(const FInputActionValue& Value);
	// ~ End of Input Binding

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

private:

	TArray<uint8> InputHandles;

	UPROPERTY()
	TArray<TObjectPtr<UInteractEntry>> _InteractPool;

};

