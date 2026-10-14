// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Blueprint/UserWidget.h"

// Generated Headers
#include "QuestDetailUI.generated.h"

// Forward Declarations
class UWidgetSwitcher;
class UTextBlock;
class UImage;
class UButton;
class UPanelWidget;
class UAssetManager;
class UQuestSubsystem;
class UQuestEntry;
class UQuestRuntimeDataCollectionUI;


/**
 *
 */
UCLASS(Abstract)
class UQuestDetailUI : public UUserWidget
{

	GENERATED_BODY()

public:

	UQuestDetailUI(const FObjectInitializer& ObjectInitializer);

	TDelegate<void()> OnCloseRequest;


	void ShowDetail(UQuestEntry* Entry);
	void HideDetail();

	bool HandleOnKeyDown(const FKeyEvent& InKeyEvent);

#if WITH_EDITORONLY_DATA
	// ~ UPrimaryDataAsset
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
	// ~ End of UPrimaryDataAsset
#endif

protected:

	UPROPERTY(EditAnywhere)
	FKey TrackKey;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> DetailSwitch = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> QuestTitle = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> QuestDescription = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> QuestIcon = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> TrackButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UQuestRuntimeDataCollectionUI> QuestData = nullptr;

	UPROPERTY()
	TObjectPtr<UQuestSubsystem> QuestSubsystem;

	UPROPERTY()
	TObjectPtr<UAssetManager> AssetManager;

	FPrimaryAssetId QuestAssetId;
	bool bQuestIsActive = false;


	void ShowPrimaryDetail(FPrimaryAssetId AssetId);
	void ShowSecondayDetail(UQuestEntry* Entry);

	// ~ Binding
	UFUNCTION()
	void HandleOnTrackClicked();
	// ~ End of Binding

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};

