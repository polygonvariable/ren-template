// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Generated Headers
#include "LuauSourceCode.generated.h"


/*
 *
 */
USTRUCT(BlueprintType)
struct FLuauSourceCode
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FString Code;

	UPROPERTY(VisibleAnywhere)
	TArray<uint8> Bytecode;

	const FString& Get() const
	{
		return Code;
	}

	bool IsValid() const
	{
		return Code.Len() > 0;
	}

};

