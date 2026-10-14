// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Blueprint/UserWidget.h"

// Generated Headers
#include "QuestTrackUI.generated.h"

// Forward Declarations
class UInputAction;
class UQuestSubsystem;
class UQuestRuntimeDataCollectionUI;
struct FInputActionValue;


/**
 *
 */
UCLASS(Abstract)
class UQuestTrackUI : public UUserWidget
{

	GENERATED_BODY()

public:

	UQuestTrackUI(const FObjectInitializer& ObjectInitializer);

#if WITH_EDITORONLY_DATA
	// ~ UPrimaryDataAsset
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
	// ~ End of UPrimaryDataAsset
#endif

protected:

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> CancelInput = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UQuestRuntimeDataCollectionUI> QuestData = nullptr;

	UPROPERTY()
	TObjectPtr<UQuestSubsystem> QuestSubsystem;


	// ~ Binding
	void HandleOnQuestDataChanged();
	void HandleOnQuestCancelInput(const FInputActionValue& Value);
	// ~ End of Binding

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

private:

	TArray<uint8> InputHandles;

};

