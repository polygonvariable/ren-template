// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "ComponentTemplateData.h"
#include "InteractComponent.h"

// Generated Headers
#include "InteractTemplateData.generated.h"


/*
 *
 */
USTRUCT(DisplayName = "Interact Component")
struct FInteractTemplateData : public FComponentTemplateData
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FGuid InteractId;

	UPROPERTY(EditAnywhere)
	FInteractItem InteractItem;

	UPROPERTY(EditAnywhere, meta = (Categories = "GameContext"))
	FGameplayTagContainer ContextTags;

	UPROPERTY(EditAnywhere)
	bool bHideAfterInteract = true;

    // ~ FComponentTemplateData
    virtual TSubclassOf<UActorComponent> GetComponentClass() const override
    {
        return UInteractComponent::StaticClass();
    };

    virtual bool ApplyToInstance(UObject* Target) override
    {
        UInteractComponent* Component = Cast<UInteractComponent>(Target);
        if (!::IsValid(Component))
        {
            return false;
        }
        Component->InteractId = InteractId;
        Component->InteractItem = InteractItem;
        Component->ContextTags = ContextTags;
        Component->bHideAfterInteract = bHideAfterInteract;
        return true;
    };
    // ~ End of FComponentTemplateData

};

