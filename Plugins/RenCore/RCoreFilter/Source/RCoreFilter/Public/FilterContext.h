// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers

// Generated Headers
#include "FilterContext.generated.h"

// Forward Declarations



/**
 *
 */
UENUM(BlueprintType)
enum class EFilterCombination : uint8
{
	And UMETA(DisplayName = "And"),
	Or UMETA(DisplayName = "Or")
};



/**
 *
 */
USTRUCT()
struct FFilterContext
{

	GENERATED_BODY()

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

	RCOREFILTER_API void SetTypedValue(FName Key, FName Value);
	RCOREFILTER_API void SetTypedValue(FName Key, int Value);
	RCOREFILTER_API void SetTypedValue(FName Key, FPrimaryAssetId Value);

};

