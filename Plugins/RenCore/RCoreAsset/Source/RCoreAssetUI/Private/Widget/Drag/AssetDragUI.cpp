// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/Drag/AssetDragUI.h"

// Engine Headers
#include "Components/Image.h"
#include "Components/TextBlock.h"

// Project Headers
#include "Data/CoreDataAsset.h"
#include "Widget/Drag/AssetDragOperation.h"



void UAssetDragUI::NativeConstruct()
{
	Super::NativeConstruct();

	UAssetDragOperation* DragOperation = Cast<UAssetDragOperation>(GetOuter());
	if (!IsValid(DragOperation))
	{
		return;
	}

	const UCoreDataAsset* DataAsset = DragOperation->DataAsset;
	if (!IsValid(DataAsset))
	{
		return;
	}

	AssetDisplayName->SetText(DataAsset->DisplayName);
	AssetIcon->SetBrushFromSoftTexture(DataAsset->Icon);
}

