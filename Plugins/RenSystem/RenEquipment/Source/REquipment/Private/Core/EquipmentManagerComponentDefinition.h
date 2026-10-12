// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "ComponentTemplateDefinition.h"
#include "EquipmentManagerComponent.h"
#include "Core/Type/EquipmentSpawnData.h"
#include "SpawnDataSource.h"

// Generated Headers
#include "EquipmentManagerComponentDefinition.generated.h"


/*
 *
 */
USTRUCT(DisplayName = "Equipment Manager Component")
struct FEquipmentManagerComponentDefinition : public FComponentTemplateDefinition
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	ESpawnDataSource SpawnSource = ESpawnDataSource::Static;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "SpawnSource==ESpawnDataSource::Static", EditConditionHides))
	TArray<FEquipmentInitializationData> SpawnData;

protected:
    
    // ~ FComponentTemplateDefinition
    virtual TSubclassOf<UActorComponent> GetComponentClass() const override
    {
        return UEquipmentManagerComponent::StaticClass();
    };

    virtual void InitializeInstance(UActorComponent* Target) override
    {
        UEquipmentManagerComponent* Component = Cast<UEquipmentManagerComponent>(Target);
        checkf(::IsValid(Component), TEXT("Equipment manager template target is invalid or not of valid type"));

        Component->SpawnSource = SpawnSource;
        Component->EquipmentSpawnData = SpawnData;
    };
    // ~ End of FComponentTemplateDefinition

};

