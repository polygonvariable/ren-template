// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

// Generated Headers
#include "FilterRule.generated.h"



/**
 *
 */
USTRUCT(BlueprintType)
struct FFilterRule
{

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Filter Rule")
	bool bEnable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Filter Rule")
	bool bEnableStrictMode = true;

};


/**
 *
 */
USTRUCT(BlueprintType)
struct FFilterNameRule : public FFilterRule
{

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Filter Rule")
	TArray<FName> Included;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Filter Rule")
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
USTRUCT(BlueprintType)
struct FFilterUInt8Rule : public FFilterRule
{

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Filter Rule")
	TArray<uint8> Included;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Filter Rule")
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
USTRUCT(BlueprintType)
struct FFilterIntegerRule : public FFilterRule
{

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Filter Rule")
	int Min = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Filter Rule")
	int Max = 0;

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
UENUM(BlueprintType)
enum class EFilterCombination : uint8
{
	And UMETA(DisplayName = "And"),
	Or UMETA(DisplayName = "Or")
};

