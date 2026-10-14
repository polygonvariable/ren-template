// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "UObject/Interface.h"
#include "StructUtils/InstancedStruct.h"

// Generated Headers
#include "FilterUIInterface.generated.h"

// Delegate Declaration
DECLARE_DELEGATE_OneParam(FOnFilterSelected, const FInstancedStruct& Data);


UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UFilterUIInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class CFILTER_API IFilterUIInterface
{

	GENERATED_BODY()

public:

	virtual void SetSelected(int Index) = 0;
	FOnFilterSelected& GetOnFilterSelected() { return OnFilterSelected; }

protected:

	FOnFilterSelected OnFilterSelected;

};

