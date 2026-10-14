// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "StructUtils/InstancedStruct.h"

// Project Headers
#include "FilterUIInterface.h"
#include "FilterGroup.h"

// Generated Headers
#include "TabWidget.generated.h"

// Forward Declarations
class UPanelWidget;
class UImage;
class UTexture2D;
class USlateBrushAsset;



/**
 *
 */
UCLASS(Abstract, DisplayName = "Tab Control")
class UTabControl : public UUserWidget
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, Category = "Tab Control|Runtime")
	uint8 TabIndex = 0;


	UPROPERTY(BlueprintReadWrite, Category = "Tab Control|Runtime")
	FText TabTitle;


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (ForceAsFunction), Category = "Tab Control|Action")
	void SetSelected(bool bIsSelected = false);
	virtual void SetSelected_Implementation(bool bIsSelected = false);

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Tab Control|Property")
	FColor TabInactiveColor = FColor::White;


	UPROPERTY(BlueprintReadOnly, Category = "Tab Control|Property")
	FColor TabActiveColor = FColor::Red;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Tab Control|Binding")
	TObjectPtr<UButton> TabButton;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Tab Control|Binding")
	TObjectPtr<UTextBlock> TabTextBlock;


	UPROPERTY(BlueprintReadWrite, Category = "Tab Control|Runtime")
	bool bSelected = false;


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (ForceAsFunction, BlueprintProtected), Category = "Tab Control|Handler")
	void HandleSelection();
	virtual void HandleSelection_Implementation();

protected:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelected, UTabControl*, TabControl);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Tab Control|Event Dispatcher")
	FOnSelected OnSelected;

};


/**
 *
 */
UCLASS(Abstract, DisplayName = "TabBox Control")
class UTabBoxControl : public UUserWidget
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional), Category = "TabBox Control|Binding")
	TObjectPtr<UPanelWidget> TabContainer;


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (ForceAsFunction), Category = "TabBox Control|Action")
	void BuildTabs(const TMap<uint8, FName>& TabItems);
	virtual void BuildTabs_Implementation(const TMap<uint8, FName>& TabItems);


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (ForceAsFunction), Category = "TabBox Control|Action")
	void ClearTabs();
	virtual void ClearTabs_Implementation();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TabBox Control|Item")
	TSubclassOf<UTabControl> TabControlClass = UTabControl::StaticClass();


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (ForceAsFunction), Category = "TabBox Control|Action")
	void HandleTabSelected(UTabControl* SelectedTab);
	virtual void HandleTabSelected_Implementation(UTabControl* SelectedTab);


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (ForceAsFunction, BlueprintProtected), Category = "TabBox Control|Handler")
	void HandleTabSetup(UTabControl* NewTab);
	virtual void HandleTabSetup_Implementation(UTabControl* NewTab);


public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTabChanged, uint8, TabIndex);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "TabBox Control|Event Dispatcher")
	FOnTabChanged OnTabChanged;

};


/*
 *
 */
UCLASS(HideCategories = "Appearance")
class UKeyActionButton : public UButton
{

	GENERATED_BODY()

public:

	UKeyActionButton();

	UFUNCTION(BlueprintCallable)
	void SetLabel(FText NewLabel);

protected:

	UPROPERTY(EditAnywhere)
	FText LabelText;

	UPROPERTY(EditAnywhere)
	FText CardText;


	UPROPERTY(EditAnywhere)
	TObjectPtr<USlateWidgetStyleAsset> LabelTextStyle;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USlateBrushAsset> CardStyle;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USlateWidgetStyleAsset> CardTextStyle;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USlateWidgetStyleAsset> ButtonStyle;


	UPROPERTY(EditAnywhere)
	FMargin CardTextPadding;

	UPROPERTY(EditAnywhere)
	FMargin CardSlotPadding;

	UPROPERTY(EditAnywhere)
	FMargin TextSlotPadding;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EHorizontalAlignment> HorizontalAlignment;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EVerticalAlignment> VerticalAlignment;


	UPROPERTY(Transient)
	TObjectPtr<UBorder> CardBorder;

	UPROPERTY(Transient)
	TObjectPtr<UTextBlock> CardTextBlock;

	UPROPERTY(Transient)
	TObjectPtr<UTextBlock> LabelTextBlock;

	// ~ UButton
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void SynchronizeProperties() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	// ~ End of UButton

private:

	void BuildDefaultContent();

};


/*
 *
 */
UCLASS(HideCategories = "Appearance")
class UIconButton : public UButton
{

	GENERATED_BODY()

public:

	UIconButton();

	UFUNCTION(BlueprintCallable)
	void SetLabel(FText NewLabel);

	UFUNCTION(BlueprintCallable)
	void SetActive(bool bActive);

protected:

	UPROPERTY(EditAnywhere)
	FText LabelText;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USlateWidgetStyleAsset> ActiveButtonStyle;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USlateWidgetStyleAsset> InactiveButtonStyle;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USlateBrushAsset> CardStyle;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USlateWidgetStyleAsset> LabelTextStyle;


	UPROPERTY(EditAnywhere)
	FMargin CardSlotPadding;

	UPROPERTY(EditAnywhere)
	FMargin TextSlotPadding;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EHorizontalAlignment> HorizontalAlignment;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EVerticalAlignment> VerticalAlignment;


	UPROPERTY(Transient)
	TObjectPtr<UImage> CardImage;

	UPROPERTY(Transient)
	TObjectPtr<UTextBlock> LabelTextBlock;

	// ~ UButton
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void SynchronizeProperties() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	// ~ End of UButton

private:

	void BuildDefaultContent();

};


/*
 *
 */
UCLASS(HideCategories = "Appearance")
class UCardContainer : public UBorder
{

	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere)
	TObjectPtr<USlateBrushAsset> CardStyle;

	// ~ UBorder
	virtual void SynchronizeProperties() override;
	// ~ End of UBorder

};


/*
 *
 */
UCLASS(HideCategories = "Appearance")
class UCardItem : public UBorder
{

	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void IsActive(bool bActive);

protected:

	UPROPERTY(EditAnywhere)
	TObjectPtr<USlateBrushAsset> ActiveStyle;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USlateBrushAsset> InactiveStyle;

	// ~ UBorder
	virtual void SynchronizeProperties() override;
	// ~ End of UBorder

};


/*
 *
 */
UCLASS(HideCategories = "Appearance")
class ULabelBlock : public UTextBlock
{

	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere)
	TObjectPtr<USlateWidgetStyleAsset> LabelStyle;

	UPROPERTY(EditAnywhere)
	int FontSize = 20;

	// ~ UBorder
	virtual void SynchronizeProperties() override;
	// ~ End of UBorder

};






/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UFilterUI : public UUserWidget
{

	GENERATED_BODY()

public:

	FOnFilterSelected OnSelected;

	UPROPERTY(EditAnywhere)
	FInstancedStruct FilterData;


	void OnSelectionChanged(bool bSelected);

protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> FilterButton = nullptr;

	UPROPERTY(EditAnywhere)
	bool bActiveInitally = false;

	UPROPERTY(EditAnywhere)
	FText FilterLabel;


	// ~ Binding
	UFUNCTION()
	void HandleOnFilterClicked();
	// ~ End of Binding

	// ~ UUserWidget
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};

/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UFilterCollectionUI : public UUserWidget, public IFilterUIInterface
{

	GENERATED_BODY()

public:

	// ~ IFilterUIInterface
	virtual void SetSelected(int Index) override;
	// ~ End of IFilterUIInterface

protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPanelWidget> FilterCollection = nullptr;


	// ~ Binding
	void HandleOnFilterSelected(const FInstancedStruct& Data, UFilterUI* Filter);
	// ~ End of Binding

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};


