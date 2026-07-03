// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Blueprint/IUserObjectListEntry.h"

// Project Headers
#include "Widget/AssetUI.h"

// Generated Headers
#include "AssetEntryUI.generated.h"

// Module Macros
#define REN_API RCOREASSETUI_API

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
	REN_API virtual void ResetDetail() override;
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
	REN_API virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	REN_API virtual void NativeOnItemSelectionChanged(bool bSelected) override;
	// ~ End of IUserObjectListEntry

	// ~ UWidget
	REN_API virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation);
	// ~ End of UWidget

};



// Module Macros
#undef REN_API

