// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "StructUtils/InstancedStruct.h"

// Project Headers
#include "Core/Type/ComponentTemplateData.h"

// Generated Headers
#include "ComponentDefinition.generated.h"

// Forward Declarations
class UActorComponent;


/*
 *
 */
USTRUCT()
struct FComponentDefinition
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UActorComponent> Component;

	UPROPERTY(EditAnywhere, meta = (ExcludeBaseStruct))
	TInstancedStruct<FComponentTemplateData> Data;


	bool IsValid() const
	{
		return Data.IsValid() && Component != nullptr;
	}

#if WITH_EDITOR
	bool IsDataValid() const
	{
		const FComponentTemplateData* DefinitionData = Data.GetPtr<FComponentTemplateData>();
		if (!DefinitionData)
		{
			return false;
		}
		return DefinitionData->IsDataValid(Component);
	}
#endif

};

