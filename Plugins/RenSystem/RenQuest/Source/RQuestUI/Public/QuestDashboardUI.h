// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Data/QuestAsset.h"

// Project Headers
#include "Core/QuestQueryDefinition.h"

// Generated Headers
#include "QuestDashboardUI.generated.h"

// Forward Declarations
class UButton;
class UListView;
class UUserWidget;
class UAssetManager;
class UQuestSubsystem;
class UQuestDetailUI;


/**
 *
 */
UCLASS(Abstract)
class UQuestDashboardUI : public UUserWidget
{

	GENERATED_BODY()

public:

	UQuestDashboardUI(const FObjectInitializer& ObjectInitializer);

protected:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> CloseButton = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UListView> QuestList = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UQuestDetailUI> QuestDetail = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UUserWidget> FilterCollection = nullptr;

	UPROPERTY(EditAnywhere)
	FKey CloseKey;

	UPROPERTY(EditAnywhere)
	FQuestQueryDefinition QuestQuery;

	UPROPERTY()
	TObjectPtr<UQuestSubsystem> QuestSubsystem;

	UPROPERTY()
	TObjectPtr<UAssetManager> AssetManager;


	// ~ Binding
	UFUNCTION()
	void HandleOnCloseClicked();

	UFUNCTION()
	void HandleOnVisibilityChanged(ESlateVisibility Visiblity);

	void HandleOnFilterSelected(const FInstancedStruct& Data);
	void HandleOnQuestChanged();
	void HandleOnQuestSelectionChanged(UObject* Object);
	// ~ End of Binding

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	// ~ End of UUserWidget

};

