// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Blueprint/IUserObjectListEntry.h"

// Project Headers
#include "Widget/AssetUI.h"

// Generated Headers
#include "AssetEntryUI.generated.h"

// Forward Declarations
class UImage;
class UTextBlock;
class UAssetDragOperation;
struct FInstancedStruct;


/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UAssetEntryUI : public UAssetUI, public IUserObjectListEntry
{

	GENERATED_BODY()

public:

	// ~ UAssetUI
	CASSETUI_API virtual void ResetDetail() override;
	// ~ End of UAssetUI

protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> EntryIcon = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> EntryName = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAssetDragOperation> DragOperationClass = nullptr;


	UFUNCTION(BlueprintCallable)
	void GetAssetSubDetail(FInstancedStruct& SubDetail) const;

	// ~ IUserObjectListEntry
	CASSETUI_API virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	CASSETUI_API virtual void NativeOnItemSelectionChanged(bool bSelected) override;
	// ~ End of IUserObjectListEntry

	// ~ UWidget
	CASSETUI_API virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation);
	// ~ End of UWidget

};

