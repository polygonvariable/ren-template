// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "InstancedStruct.h"

// Project Headers
#include "Record/RenRecord.h"

// Generated Headers
#include "EnhanceRecord.generated.h"


/**
 * 
 */
USTRUCT(BlueprintType, DisplayName = "Enhance Record")
struct FEnhanceRecord : public FRenRecord
{

	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Rank = 1;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Level = 1;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Experience = 0;

};

