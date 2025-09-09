// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "UObject/ObjectSaveContext.h"

// Project Headers

// Generated Headers
#include "EventflowAsset.generated.h"

// Forward Declarations
class UEventflowData;
class UEventflowBlueprint;



/**
 *
 *
 */
UCLASS(BlueprintType)
class RENEVENTFLOW_API UEventflowAsset : public UDataAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UEventflowBlueprint> GraphBlueprint;

	UPROPERTY()
	TObjectPtr<UEventflowData> GraphData = nullptr;

#if WITH_EDITORONLY_DATA

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseMultiEntry = false;

#endif

#if WITH_EDITOR

	DECLARE_MULTICAST_DELEGATE(FOnEventflowAssetSaved);
	FOnEventflowAssetSaved OnEventflowAssetSaved;

	virtual void PostPropertyUpdate();

protected:

	// ~ UObject
	virtual void PreSaveRoot(FObjectPreSaveRootContext ObjectSaveContext) override;
	virtual void PreSave(FObjectPreSaveContext ObjectSaveContext) override;
	// ~ End of UObject

public:

	// ~ UObject
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const;
	// ~ End of UObject

	bool ValidateGraphData(FString& Message) const;
	bool ValidateGraphNodes(FString& Message) const;

#endif

};

