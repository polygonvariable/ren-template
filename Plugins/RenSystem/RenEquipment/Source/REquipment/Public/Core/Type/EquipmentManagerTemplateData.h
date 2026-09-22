// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Core/Type/ComponentTemplateData.h"
#include "EquipmentManagerComponent.h"
#include "Core/Type/EquipmentSpawnData.h"
#include "Definition/QueryType.h"

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
	EDataSource SourceType = EDataSource::Static;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "SourceType==EDataSource::Static", EditConditionHides))
	TArray<FEquipmentInitializationData> SpawnData;

    
    virtual bool IsDataValid(UClass* Target) const override
    {
        return Target == UEquipmentManagerComponent::StaticClass();
    };

    virtual bool ApplyToInstance(UObject* Target) override
    {
        UEquipmentManagerComponent* Component = Cast<UEquipmentManagerComponent>(Target);
        if (!IsValid(Component))
        {
            return false;
        }
        Component->SourceType = SourceType;
        Component->EquipmentSpawnData = SpawnData;
        return true;
    };

};

