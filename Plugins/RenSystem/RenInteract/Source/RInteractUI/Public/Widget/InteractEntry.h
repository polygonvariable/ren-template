// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Definition/InteractItem.h"

// Generated Headers
#include "InteractEntry.generated.h"


/**
 *
 */
UCLASS()
class UInteractEntry : public UObject
{

	GENERATED_BODY()

public:

	FGuid InteractId;
	FInteractItem InteractItem;
	
	void ResetData()
	{
		InteractId.Invalidate();
		InteractItem.Reset();
	}

};

