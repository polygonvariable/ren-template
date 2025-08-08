// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

// Project Headers
#include "RenCore/Public/Record/InventoryRecord.h"

// Generated Headers
#include "EnhanceItemWidget.generated.h"

// Forward Declarations
class UInventoryAsset;
class UInventoryCollectionWidget;
class UInventoryDetailWidget;
class UInventoryEntryObject;
class UEnhanceItemSubsystem;


/**
 *
 */
UCLASS(Abstract)
class UEnhanceItemWidget : public UUserWidget
{

	GENERATED_BODY()

public:

	void InitializeDetail(FName ItemGuid, const FInventoryRecord* Record, UInventoryAsset* Asset);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ActiveItemGuid = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ContainerId = NAME_None;

	UPROPERTY();
	TWeakObjectPtr<UEnhanceItemSubsystem> EnhanceItemSubsystem;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UInventoryCollectionWidget> InventoryCollection = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UInventoryDetailWidget> InventoryDetail = nullptr;


	void TryLevelUp(FName ItemGuid);
	void TryRankUp();

	void HandleItemSelected(FName ItemGuid, const FInventoryRecord* Record, UInventoryAsset* Asset);

protected:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

};

