// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Blueprint/UserWidget.h"

// Generated Headers
#include "QuestUI.generated.h"

// Forward Declarations
class UQuestEngine;
class UQuestSubsystem;
class UEventflowNodeTask;
class UButton;
class UInputAction;
class UTextBlock;
struct FInputActionValue;


/**
 *
 */
UCLASS(Abstract)
class UQuestItemUI : public UUserWidget
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> TitleTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> NodeTaskTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> SubTaskTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UPanelWidget> GroupPanel;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UButton> CancelButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UButton> FocusButton;

	FPrimaryAssetId AssetId;


	UFUNCTION(BlueprintCallable)
	void FocusQuest();

	UFUNCTION(BlueprintCallable)
	void CancelQuest();

};


/**
 *
 */
UCLASS(Abstract)
class UQuestUI : public UUserWidget
{

	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPanelWidget> QuestPanel = nullptr;

protected:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UQuestItemUI> ItemClass;

	UPROPERTY()
	UQuestSubsystem* QuestSubsystem;


	void RenderSubTask(UEventflowNodeTask* NodeTask, UQuestItemUI* NewItem);
	void RenderNodeTask(UQuestEngine* Engine, UEventflowNodeTask* NodeTask, UQuestItemUI* NewItem);
	void RenderMainQuests(const TMap<FPrimaryAssetId, TObjectPtr<UQuestEngine>>& Quests);

	void RenderGroupSubTask(UEventflowNodeTask* NodeTask, UQuestItemUI* GroupItem);
	void RenderGroupNodeTask(UQuestEngine* Engine, UEventflowNodeTask* NodeTask, UQuestItemUI* GroupItem);
	void RenderGroupQuests(const TMap<FPrimaryAssetId, TObjectPtr<UQuestEngine>>& GroupEngines, UQuestItemUI* NewItem);

	void HandleOnQuestUpdated();

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};


/**
 *
 */
UCLASS(Abstract)
class UQuestFocusUI : public UUserWidget
{

	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPanelWidget> QuestPanel = nullptr;

protected:

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> QuestCancel = nullptr;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UQuestItemUI> ItemClass;

	UPROPERTY()
	UQuestSubsystem* QuestSubsystem;


	void RenderSubTask(UEventflowNodeTask* NodeTask, UQuestItemUI* NewItem);
	void RenderNodeTask(UQuestEngine* Engine, UEventflowNodeTask* NodeTask, UQuestItemUI* NewItem);
	void RenderMainQuests(UQuestEngine* Engine);

	void RenderGroupSubTask(UEventflowNodeTask* NodeTask, UQuestItemUI* GroupItem);
	void RenderGroupNodeTask(UQuestEngine* Engine, UEventflowNodeTask* NodeTask, UQuestItemUI* GroupItem);
	void RenderGroupQuests(const TMap<FPrimaryAssetId, TObjectPtr<UQuestEngine>>& GroupEngines, UQuestItemUI* NewItem);

	void HandleOnQuestUpdated();
	void HandleOnQuestCancelInput(const FInputActionValue& Value);

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

private:

	TArray<uint8> InputHandles;

};

