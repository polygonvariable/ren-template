// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

// Project Headers

// Generated Headers
#include "InteractWidget.generated.h"

// Forward Declarations
class UPanelWidget;
class UTextBlock;

class UInteractSubsystem;
class IInteractInterface;

struct FInteractItem;



/**
 *
 */
UCLASS(Abstract)
class UInteractEntryWidget : public UUserWidget
{

	GENERATED_BODY()

public:

	void InitializeDetails(AActor* Actor, const FInteractItem* Item);
	void ResetDetails();

protected:

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> InteractTitle;

	TWeakInterfacePtr<IInteractInterface> InteractProvider;


	UFUNCTION(BlueprintCallable)
	void SelectEntry();

	void HandleItemDetails(const FInteractItem* Item);

protected:

	virtual void NativeDestruct() override;

};



/**
 *
 */
UCLASS(Abstract)
class UInteractWidget : public UUserWidget
{

	GENERATED_BODY()

protected:

	UPROPERTY()
	TWeakObjectPtr<UInteractSubsystem> InteractSubsystem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UInteractEntryWidget> InteractEntryClass;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UPanelWidget> InteractEntryPanel;

	TMap<TWeakObjectPtr<AActor>, TObjectPtr<UInteractEntryWidget>> InteractEntries;


	virtual void HandleInteractStarted(AActor* Actor, const FInteractItem* Item);
	virtual void HandleInteractEnded(AActor* Actor);

protected:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

};

