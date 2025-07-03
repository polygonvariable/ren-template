// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "InstancedStruct.h"

// Module Headers

// Generated Headers
#include "RenRecord.generated.h"


/**
 * 
 */
USTRUCT(BlueprintType, DisplayName = "Ren Record")
struct FRenRecord
{

	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FInstancedStruct CustomRecord;

};

