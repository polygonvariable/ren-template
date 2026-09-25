// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Core/Type/ComponentTemplateData.h"
#include "Component/HealthManagerComponent.h"

// Generated Headers
#include "HealthManagerTemplateData.generated.h"


/*
 *
 */
USTRUCT(DisplayName = "Health Manager Component")
struct FHealthManagerTemplateData : public FComponentTemplateData
{

	GENERATED_BODY()

public:

    virtual bool IsDataValid(UClass* Target) const override
    {
        return Target == UHealthManagerComponent::StaticClass();
    };

    virtual bool ApplyToInstance(UObject* Target) override
    {
        UHealthManagerComponent* Component = Cast<UHealthManagerComponent>(Target);
        if (!IsValid(Component))
        {
            return false;
        }
        return true;
    };

};

