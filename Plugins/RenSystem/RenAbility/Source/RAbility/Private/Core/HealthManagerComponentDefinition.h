// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "ComponentTemplateDefinition.h"
#include "Component/HealthManagerComponent.h"

// Generated Headers
#include "HealthManagerComponentDefinition.generated.h"


/*
 *
 */
USTRUCT(DisplayName = "Health Manager Component")
struct FHealthManagerComponentDefinition : public FComponentTemplateDefinition
{

	GENERATED_BODY()

protected:

    // ~ FComponentTemplateDefinition
    virtual TSubclassOf<UActorComponent> GetComponentClass() const override
    {
        return UHealthManagerComponent::StaticClass();
    };

    virtual void InitializeInstance(UActorComponent* Target) override
    {
        UHealthManagerComponent* Component = Cast<UHealthManagerComponent>(Target);
        checkf(::IsValid(Component), TEXT("Health manager template target is invalid or not of valid type"));
    };
    // ~ End of FComponentTemplateDefinition

};

