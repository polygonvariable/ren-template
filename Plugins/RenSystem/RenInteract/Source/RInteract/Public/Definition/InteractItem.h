// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Generated Headers
#include "InteractItem.generated.h"


/**
 *
 */
USTRUCT(BlueprintType)
struct FInteractItem
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText InteractTitle;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UTexture2D> InteractIcon = nullptr;

	void Reset()
	{
		InteractTitle = FText::GetEmpty();
		InteractIcon = nullptr;
	}

	friend inline bool operator == (const FInteractItem& A, const FInteractItem& B)
	{
		return A.InteractTitle.ToString() == B.InteractTitle.ToString() && A.InteractIcon == B.InteractIcon;
	}

	friend inline uint32 GetTypeHash(const FInteractItem& A)
	{
		return HashCombineFast(GetTypeHash(A.InteractTitle.ToString()), GetTypeHash(A.InteractIcon));
	}

};

