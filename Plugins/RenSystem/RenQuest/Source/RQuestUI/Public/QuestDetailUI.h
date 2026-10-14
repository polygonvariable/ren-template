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

DECLARE_DELEGATE(FOnWidgetCloseRequest)

/**
 *
 */
UCLASS(Abstract)
class UQuestDetailUI : public UUserWidget
{

	GENERATED_BODY()

public:

	UQuestDetailUI(const FObjectInitializer& ObjectInitializer);

	FOnWidgetCloseRequest OnCloseRequest;


	void ShowDetail(UQuestEntry* Entry);
	void HideDetail();

	bool HandleOnKeyDown(const FKeyEvent& InKeyEvent);

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

class UQuestEngine;
class UQuestPrimaryTask;
class UQuestSubTask;
struct FQuestRuntimeData;

UCLASS(Abstract)
class UQuestRuntimeDataUI : public UUserWidget
{

	GENERATED_BODY()

public:

	void ShowPrimaryDetail(const FText& Title, const FText& Description);
	void ShowRuntimeDetail(const FQuestRuntimeData* Data);

protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TitleText = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DescriptionText = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ValueText = nullptr;

};


UCLASS(Abstract)
class UQuestRuntimeDataCollectionUI : public UUserWidget
{

	GENERATED_BODY()

public:

	UQuestRuntimeDataCollectionUI(const FObjectInitializer& ObjectInitializer);

	FOnWidgetCloseRequest OnCloseRequest;


	bool HandleOnKeyDown(const FKeyEvent& InKeyEvent);

	void ShowDetail(UQuestEngine* Engine, const FPrimaryAssetId& AssetId);
	void HideDetail();

protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CancelButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPanelWidget> DataPanel = nullptr;

	UPROPERTY(EditAnywhere)
	FKey CancelKey;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UQuestRuntimeDataUI> QuestDataClass;

	UPROPERTY()
	TObjectPtr<UQuestSubsystem> QuestSubsystem;

	FPrimaryAssetId QuestAssetId;


	void ShowEngineDetail(UQuestEngine* Engine, const FPrimaryAssetId& AssetId);
	void ShowNodeDetail(UQuestPrimaryTask* NodeTask, const FPrimaryAssetId& AssetId);
	void ShowSubDetail(UQuestPrimaryTask* NodeTask);
	void ShowGroupEngines(UQuestEngine* Engine);

	// ~ Binding
	UFUNCTION()
	void HandleOnCancelClicked();
	// ~ End of Binding

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};

