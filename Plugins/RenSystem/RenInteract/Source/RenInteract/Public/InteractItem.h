// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText InteractTitle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> InteractIcon = nullptr;

	friend inline bool operator == (const FInteractItem& A, const FInteractItem& B)
	{
		return A.InteractGuid == B.InteractGuid;
	}

	friend inline uint32 GetTypeHash(const FInteractItem& Item)
	{
		return GetTypeHash(Item.InteractGuid);
	}

private:

	FGuid InteractGuid = FGuid::NewGuid();

};

