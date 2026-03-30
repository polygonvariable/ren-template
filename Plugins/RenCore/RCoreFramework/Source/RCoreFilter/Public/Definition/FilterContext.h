// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers

// Generated Headers
#include "FilterContext.generated.h"

// Module Macros
#define REN_API RCOREFILTER_API

// Forward Declarations



/**
 *
 */
USTRUCT()
struct FFilterContext
{

	GENERATED_BODY()

public:

	template<typename T>
	void SetValue(FName Key, T Value)
	{
		SetTypedValue(Key, Value);
	}

	template<typename T>
	bool GetValue(FName Key, T& OutValue) const
	{
		return GetTypedValue(Key, OutValue);
	}

protected:

	TArray<TPair<FName, FName>> TextProperties;
	TArray<TPair<FName, int>> IntProperties;
	TArray<TPair<FName, FPrimaryAssetId>> AssetProperties;

private:

	bool GetTypedValue(FName Key, FName& OutValue) const;
	bool GetTypedValue(FName Key, int& OutValue) const;
	bool GetTypedValue(FName Key, FPrimaryAssetId& OutValue) const;

	REN_API void SetTypedValue(FName Key, FName Value);
	REN_API void SetTypedValue(FName Key, int Value);
	REN_API void SetTypedValue(FName Key, FPrimaryAssetId Value);

};



// Module Macros
#undef REN_API

