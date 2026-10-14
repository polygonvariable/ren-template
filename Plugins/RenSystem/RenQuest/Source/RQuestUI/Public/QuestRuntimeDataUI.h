// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Blueprint/UserWidget.h"

// Generated Headers
#include "QuestRuntimeDataUI.generated.h"

// Forward Declarations
class UTextBlock;
class UButton;
class UPanelWidget;
class UQuestSubsystem;
class UQuestEngine;
class UQuestPrimaryTask;
struct FQuestRuntimeData;


/**
 *
 */
UCLASS(Abstract)
class UQuestRuntimeDataUI : public UUserWidget
{

	GENERATED_BODY()

public:

	void SetPrimaryDetail(const FText& Title, const FText& Description);
	void SetRuntimeDetail(const FQuestRuntimeData* Data);

protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TitleText = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DescriptionText = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ValueText = nullptr;

};


/**
 *
 */
UCLASS(Abstract)
class UQuestRuntimeDataCollectionUI : public UUserWidget
{

	GENERATED_BODY()

public:

	UQuestRuntimeDataCollectionUI(const FObjectInitializer& ObjectInitializer);

	TDelegate<void()> OnCloseRequest;


	void CancelObjective();
	bool HandleOnKeyDown(const FKeyEvent& InKeyEvent);

	void ShowDetail(UQuestEngine* Engine);
	void HideDetail();

#if WITH_EDITORONLY_DATA
	// ~ UPrimaryDataAsset
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
	// ~ End of UPrimaryDataAsset
#endif

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

	UPROPERTY()
	TArray<TObjectPtr<UQuestRuntimeDataUI>> QuestDataPool;

	FPrimaryAssetId QuestAssetId;


	void ShowEngineDetail(UQuestEngine* Engine, const FPrimaryAssetId& AssetId);
	void ShowNodeDetail(UQuestPrimaryTask* NodeTask, const FPrimaryAssetId& AssetId);
	void ShowSubDetail(UQuestPrimaryTask* NodeTask);
	void RenderDetail(const FText& Title, const FText& Description, const FQuestRuntimeData* Data);
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

