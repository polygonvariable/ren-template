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



UCLASS(BlueprintType)
class RENEVENTFLOW_API UEventflowAsset : public UDataAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UEventflowBlueprint> GraphBlueprint;

	UPROPERTY()
	TObjectPtr<UEventflowData> GraphData = nullptr;

#if WITH_EDITOR

	DECLARE_MULTICAST_DELEGATE(FOnEventflowAssetSaved);
	FOnEventflowAssetSaved OnEventflowAssetSaved;

protected:

	virtual void PreSaveRoot(FObjectPreSaveRootContext ObjectSaveContext) override;
	virtual void PreSave(FObjectPreSaveContext ObjectSaveContext) override;

public:

	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const;

#endif

};

