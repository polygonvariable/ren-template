// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "ComponentDefinition.h"


bool FComponentDefinition::IsValid() const
{
    const FComponentTemplateData* DefinitionData = Data.GetPtr<FComponentTemplateData>();
    if (!DefinitionData)
    {
        return false;
    }
    return DefinitionData->IsValid();
}

