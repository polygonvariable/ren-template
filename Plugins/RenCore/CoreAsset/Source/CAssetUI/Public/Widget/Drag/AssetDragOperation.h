// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Blueprint/DragDropOperation.h"

// Generated Headers
#include "AssetDragOperation.generated.h"

// Forward Declarations
class UCoreDataAsset;



/**
 *
 */
UCLASS(MinimalAPI)
class UAssetDragOperation : public UDragDropOperation
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly)
	FPrimaryAssetId AssetId;

	UPROPERTY(BlueprintReadOnly)
	FGuid AssetInstanceId;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<const UCoreDataAsset> DataAsset;

};

