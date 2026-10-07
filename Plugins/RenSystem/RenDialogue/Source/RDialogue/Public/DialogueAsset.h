// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#if WITH_EDITOR
#include "Blueprint/UserWidget.h"
#include "Misc/DataValidation.h"
#endif

// Project Headers
#include "DialogueData.h"
#include "EventflowAsset.h"

// Generated Headers
#include "DialogueAsset.generated.h"

// Forward Declarations
class UUserWidget;


/**
 * 
 */
UCLASS(MinimalAPI, BlueprintType)
class UDialogueAsset : public UEventflowAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Dialogue", meta = (MultiLine = true))
	FText Summary;

	UPROPERTY(EditAnywhere, Category = "Dialogue")
	TArray<FDialogueSpeaker> Speakers;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> DialogueWidget;

	UPROPERTY(EditAnywhere, Category = "Widget")
	FGuid SkipNodeId;

	// ~ UPrimaryDataAsset
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(TEXT("Dialogue"), GetFName());
	}
	// ~ End of UPrimaryDataAsset

#if WITH_EDITOR
	// ~ UUserWidget
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override
	{
		EDataValidationResult Result = Super::IsDataValid(Context);

		if (!DialogueWidget)
		{
			Context.AddError(FText::FromString("Dialogue widget is invalid"));
			return EDataValidationResult::Invalid;
		}

		return Result;
	}
	// ~ End of UUserWidget
#endif

};

