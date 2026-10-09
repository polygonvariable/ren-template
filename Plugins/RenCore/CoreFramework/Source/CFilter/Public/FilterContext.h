// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Generated Headers
#include "FilterContext.generated.h"


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

	CFILTER_API bool GetTypedValue(FName Key, FName& OutValue) const;
	CFILTER_API bool GetTypedValue(FName Key, int& OutValue) const;
	CFILTER_API bool GetTypedValue(FName Key, FPrimaryAssetId& OutValue) const;

	CFILTER_API void SetTypedValue(FName Key, FName Value);
	CFILTER_API void SetTypedValue(FName Key, int Value);
	CFILTER_API void SetTypedValue(FName Key, FPrimaryAssetId Value);

};

