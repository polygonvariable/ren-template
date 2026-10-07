// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "StructUtils/InstancedStruct.h"

// Project Headers
#include "ComponentTemplateData.h"

// Generated Headers
#include "ComponentDefinition.generated.h"


/*
 *
 */
USTRUCT()
struct FComponentDefinition
{

	GENERATED_BODY()

public:

	/** if true then component wont be created, but it will try to find the first match by class and modify its values */
	UPROPERTY(EditAnywhere)
	bool bModifyExisting = false;

	UPROPERTY(EditAnywhere, meta = (ExcludeBaseStruct))
	TInstancedStruct<FComponentTemplateData> Data;

	CGAMEDATACOMPONENT_API bool IsValid() const;

};

