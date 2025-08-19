// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

// Generated Headers
#include "TabWidget.generated.h"

// Forward Declarations
class UButton;
class UTextBlock;
class UPanelWidget;


/**
 *
 */
UCLASS(Abstract, DisplayName = "Tab Control")
class RENWIDGET_API UTabControl : public UUserWidget
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, Category = "Tab Control|Runtime")
	uint8 TabIndex = 0;


	UPROPERTY(BlueprintReadWrite, Category = "Tab Control|Runtime")
	FText TabTitle;


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Meta = (ForceAsFunction), Category = "Tab Control|Action")
	void SetSelected(bool bIsSelected = false);
	virtual void SetSelected_Implementation(bool bIsSelected = false);

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Tab Control|Property")
	FColor TabInactiveColor = FColor::White;


	UPROPERTY(BlueprintReadOnly, Category = "Tab Control|Property")
	FColor TabActiveColor = FColor::Red;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (BindWidgetOptional), Category = "Tab Control|Binding")
	TObjectPtr<UButton> TabButton;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (BindWidgetOptional), Category = "Tab Control|Binding")
	TObjectPtr<UTextBlock> TabTextBlock;


	UPROPERTY(BlueprintReadWrite, Category = "Tab Control|Runtime")
	bool bSelected = false;


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Meta = (ForceAsFunction, BlueprintProtected), Category = "Tab Control|Handler")
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
class RENWIDGET_API UTabBoxControl : public UUserWidget
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (BindWidgetOptional), Category = "TabBox Control|Binding")
	TObjectPtr<UPanelWidget> TabContainer;


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Meta = (ForceAsFunction), Category = "TabBox Control|Action")
	void BuildTabs(const TMap<uint8, FName>& TabItems);
	virtual void BuildTabs_Implementation(const TMap<uint8, FName>& TabItems);


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Meta = (ForceAsFunction), Category = "TabBox Control|Action")
	void ClearTabs();
	virtual void ClearTabs_Implementation();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TabBox Control|Item")
	TSubclassOf<UTabControl> TabControlClass = UTabControl::StaticClass();


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Meta = (ForceAsFunction), Category = "TabBox Control|Action")
	void HandleTabSelected(UTabControl* SelectedTab);
	virtual void HandleTabSelected_Implementation(UTabControl* SelectedTab);


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Meta = (ForceAsFunction, BlueprintProtected), Category = "TabBox Control|Handler")
	void HandleTabSetup(UTabControl* NewTab);
	virtual void HandleTabSetup_Implementation(UTabControl* NewTab);


public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTabChanged, uint8, TabIndex);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "TabBox Control|Event Dispatcher")
	FOnTabChanged OnTabChanged;

};

