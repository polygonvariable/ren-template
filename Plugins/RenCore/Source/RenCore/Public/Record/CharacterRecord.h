// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "InstancedStruct.h"

// Project Headers
#include "Record/RenRecord.h"
#include "Record/EnhanceRecord.h"

// Generated Headers
#include "CharacterRecord.generated.h"


/**
 * 
 */
USTRUCT(BlueprintType, DisplayName = "Character Record")
struct FCharacterRecord : public FRenRecord
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Health = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FEnhanceRecord EnhanceRecord;

};

