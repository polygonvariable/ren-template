// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "FilterRule.generated.h"


/**
 *
 */
USTRUCT(BlueprintType, DisplayName = "Filter Name Rule")
struct FFilterNameRule
{

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableStrictMode = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> Included;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> Excluded;

	bool Matches(const FName& Value) const
	{
		if (!bEnable) return true;
		if (Excluded.Contains(Value)) return false;
		if (bEnableStrictMode && Included.Num() == 0) return false;
		if (Included.Num() > 0) return Included.Contains(Value);
		return true;
	}

};


/**
 *
 */
USTRUCT(BlueprintType, DisplayName = "Filter Byte Rule")
struct FFilterUInt8Rule
{

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableStrictMode = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<uint8> Included;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<uint8> Excluded;

	bool Matches(const uint8& Value) const
	{
		if (!bEnable) return true;
		if (Excluded.Contains(Value)) return false;
		if (bEnableStrictMode && Included.Num() == 0) return false;
		if (Included.Num() > 0) return Included.Contains(Value);
		return true;
	}

};

/**
 *
 */
USTRUCT(BlueprintType, DisplayName = "Filter Integer Rule")
struct FFilterIntegerRule
{

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableStrictMode = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Min = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Max = 0;

	bool Matches(const int& Value) const
	{
		if (!bEnable) return true;
		if (bEnableStrictMode)
		{
			return Value > Min && Value < Max;
		}
		else
		{
			return Value >= Min && Value <= Max;
		}
	}

};


/**
 *
 */
UENUM(BlueprintType, DisplayName = "Filter Combination")
enum class EFilterCombination : uint8
{
	And UMETA(DisplayName = "AND"),
	Or UMETA(DisplayName = "OR")
};

