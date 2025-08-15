// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "InstancedStruct.h"

// Project Headers
#include "Record/RenRecord.h"
//#include "Record/EnhanceRecord.h"

// Generated Headers
#include "CharacterRecord.generated.h"


/**
 * 
 */
USTRUCT(BlueprintType)
struct FCharacterRecord
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Record")
	int Health = 0;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Record")
	//FEnhanceRecord EnhanceRecord;

};

