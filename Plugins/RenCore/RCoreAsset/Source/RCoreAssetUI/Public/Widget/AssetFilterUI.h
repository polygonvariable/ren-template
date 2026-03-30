// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Blueprint/UserWidget.h"

// Generated Headers
#include "AssetFilterUI.generated.h"

// Module Macros
#define REN_API RCOREASSETUI_API

// Forward Declarations
class UTextBlock;
class UButton;
class UAssetCollectionUI;
class UFilterCriterion;



/**
 *
 *
 */
USTRUCT(BlueprintType)
struct FAssetFilterCriterion
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FName FilterName = NAME_None;

	UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<UFilterCriterion> FilterCriterion = nullptr;

};



/**
 *
 *
 */
UCLASS(Abstract, MinimalAPI)
class UAssetFilterUI : public UUserWidget
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FText TitleText = FText::GetEmpty();

	DECLARE_DELEGATE(FOnSelected);
	FOnSelected OnSelected;


	// ~ UUserWidget
	REN_API virtual void NativePreConstruct() override;
	REN_API virtual void NativeConstruct() override;
	REN_API virtual void NativeDestruct() override;
	// ~ End of UUserWidget

protected:

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> FilterTitle = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> FilterButton = nullptr;


	UFUNCTION(BlueprintCallable)
	void SetSelected();

};


/**
 *
 * 
 */
UCLASS(Abstract, MinimalAPI)
class UAssetFilterCollectionUI : public UUserWidget
{

	GENERATED_BODY()

public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelected, int, Index);
	UPROPERTY(BlueprintAssignable)
	FOnSelected OnSelected;

	UPROPERTY(EditAnywhere)
	TArray<FAssetFilterCriterion> FilterCriteria;


	UFUNCTION(BlueprintCallable)
	REN_API void SetTargetCollectionUI(UAssetCollectionUI* InCollectionUI);

	// ~ UUserWidget
	REN_API virtual void NativePreConstruct() override;
	// ~ End of UUserWidget

protected:

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UPanelWidget> FilterBox = nullptr;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAssetFilterUI> FilterClass = nullptr;

	UPROPERTY(EditAnywhere)
	FMargin FilterSpacing;


	void OnFilterSelected(int Index);

private:

	UPROPERTY()
	TWeakObjectPtr<UAssetCollectionUI> _AssetCollection = nullptr;

};


// Module Macros
#undef REN_API

