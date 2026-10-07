// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "ComponentTemplateData.h"
#include "EquipmentManagerComponent.h"
#include "Core/Type/EquipmentSpawnData.h"
#include "SpawnDataSource.h"

// Generated Headers
#include "EquipmentManagerTemplateData.generated.h"


/*
 *
 */
USTRUCT(DisplayName = "Equipment Manager Component")
struct FEquipmentManagerTemplateData : public FComponentTemplateData
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	ESpawnDataSource SpawnSource = ESpawnDataSource::Static;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "SpawnSource==ESpawnDataSource::Static", EditConditionHides))
	TArray<FEquipmentInitializationData> SpawnData;

    
    // ~ FComponentTemplateData
    virtual TSubclassOf<UActorComponent> GetComponentClass() const override
    {
        return UEquipmentManagerComponent::StaticClass();
    };

    virtual bool ApplyToInstance(UObject* Target) override
    {
        UEquipmentManagerComponent* Component = Cast<UEquipmentManagerComponent>(Target);
        if (!::IsValid(Component))
        {
            return false;
        }
        Component->SpawnSource = SpawnSource;
        Component->EquipmentSpawnData = SpawnData;
        return true;
    };
    // ~ End of FComponentTemplateData

};

