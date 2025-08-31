// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Engine/Blueprint.h"

// Project Headers
#include "RenEventflow/Public/EventflowAsset.h"
#include "RenEventflow/Public/EventflowNodeData.h"

// Generated Headers
#include "DialogueAsset.generated.h"

// Forward Declarations



UCLASS(BlueprintType)
class RENDIALOGUE_API UDialogueNodeData : public UEventflowNodeData
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FText DialogueTitle;

	UPROPERTY(EditAnywhere)
	FText DialogueContent;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UTexture2D> DialogueAvatar;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<USoundBase> DialogueSound;

	UPROPERTY(EditAnywhere)
	TArray<FText> OutputOptions;

public:

	virtual const TArray<FText>* GetOutputOptions() const override;

};


UCLASS(Blueprintable, BlueprintType)
class RENDIALOGUE_API UDialogueAssetBlueprint : public UBlueprint
{

	GENERATED_BODY()

	UDialogueAssetBlueprint(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
	{
	};

#if WITH_EDITOR

	// UBlueprint interface
	virtual bool SupportedByDefaultBlueprintFactory() const override
	{
		return false;
	}
	// End of UBlueprint interface

#endif
};

UCLASS(BlueprintType)
class RENDIALOGUE_API UDialogueAsset : public UEventflowAsset
{

	GENERATED_BODY()

};

