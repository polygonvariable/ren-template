// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "ComponentTemplateDefinition.h"
#include "InteractComponent.h"

// Generated Headers
#include "InteractComponentDefinition.generated.h"


/*
 *
 */
USTRUCT(DisplayName = "Interact Component")
struct FInteractComponentDefinition : public FComponentTemplateDefinition
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

protected:

    // ~ FComponentTemplateDefinition
    virtual TSubclassOf<UActorComponent> GetComponentClass() const override
    {
        return UInteractComponent::StaticClass();
    };

    virtual void InitializeInstance(UActorComponent* Target) override
    {
        UInteractComponent* Component = Cast<UInteractComponent>(Target);
        checkf(::IsValid(Component), TEXT("Interact template target is invalid or not of valid type"));

        Component->InteractId = InteractId;
        Component->InteractItem = InteractItem;
        Component->ContextTags = ContextTags;
        Component->bHideAfterInteract = bHideAfterInteract;
    };
    // ~ End of FComponentTemplateDefinition

};

