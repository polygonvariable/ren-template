// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "Record/RenRecord.h"

// Generated Headers
#include "EnhanceRecord.generated.h"



/**
 * 
 */
USTRUCT(BlueprintType)
struct FEnhanceRecord
{

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhance Record")
	int Rank = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhance Record")
	int Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhance Record")
	int Experience = 0;

};

