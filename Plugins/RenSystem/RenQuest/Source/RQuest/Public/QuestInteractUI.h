// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Blueprint/UserWidget.h"

// Project Headers
#include "Core/IObjectiveFeedback.h"
#include "StateMachine/FiniteStateMachineType.h"
#include "WidgetTemplateDefinition.h"

// Generated Headers
#include "QuestInteractUI.generated.h"

// Forward Declarations
class UAssetCollectionUI;
class UButton;
class UOverlay;


/**
 *
 */
UCLASS(Abstract)
class UQuestInteractUI : public UUserWidget, public IObjectiveFeedback
{

	GENERATED_BODY()

protected:

	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected))
	void InteractionComplete(EFSMResult Result);


	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};

/**
 *
 */
UCLASS(Abstract)
class UQuestItemExchangeUI : public UQuestInteractUI
{

	GENERATED_BODY()

public:

	UQuestItemExchangeUI();

	UPROPERTY(EditAnywhere)
	FName InventorySourceId;

	UPROPERTY(EditAnywhere)
	FPrimaryAssetType InventoryAssetType;

	UPROPERTY(EditAnywhere)
	TMap<FPrimaryAssetId, int> InventoryItems;

#if WITH_EDITOR
	// ~ UObject
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
	// ~ End of UObject
#endif
protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAssetCollectionUI> InventoryCollection = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAssetCollectionUI> ExchangeCollection = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ConfirmButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CancelButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> MessageOverlay = nullptr;

	UPROPERTY(EditAnywhere)
	FKey ConfirmKey;

	UPROPERTY(EditAnywhere)
	FKey CancelKey;


	void RegisterDelegate();
	void UnregisterDelegate();

	void ApplyFilter();
	void DisplayItems();

	// ~ Binding
	UFUNCTION()
	void HandleOnConfirmClicked();

	UFUNCTION()
	void HandleOnCancelClicked();
	// ~ End of Binding

	// ~ UUserWidget
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	// ~ End of UUserWidget

};



/*
 *
 */
USTRUCT(DisplayName = "Quest Item Exchange")
struct FWidgetTemplate_QuestItemExchange : public FWidgetTemplateDefinition
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UQuestItemExchangeUI> WidgetClass;

	UPROPERTY(EditAnywhere)
	FName SourceId;

	UPROPERTY(EditAnywhere)
	FPrimaryAssetType AssetType;

	UPROPERTY(EditAnywhere)
	TMap<FPrimaryAssetId, int> Items;


#if WITH_EDITOR
	// ~ FInstancedTemplateDefinition
	virtual bool IsDataValid() const override;
	virtual void AppendAssetBundleData(FAssetBundleData& AssetBundle, const FName& Bundle) override;
	// ~ End of FInstancedTemplateDefinition
#endif

protected:

	// ~ FInstancedTemplateDefinition
	virtual TSubclassOf<UUserWidget> GetWidgetClass() const override;
	virtual void InitializeInstance(UUserWidget* Widget) override;
	// ~ End of FInstancedTemplateDefinition

};

