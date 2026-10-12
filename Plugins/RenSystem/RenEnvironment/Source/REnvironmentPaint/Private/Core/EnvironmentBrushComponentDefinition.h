// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "ComponentTemplateDefinition.h"

// Generated Headers
#include "EnvironmentBrushComponentDefinition.generated.h"


/*
 *
 */
USTRUCT(DisplayName = "Environment Brush Component")
struct FEnvironmentBrushComponentDefinition : public FSceneComponentTemplateDefinition
{

	GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere)
    bool bLineTrace = false;

    UPROPERTY(EditAnywhere)
    bool bCanDraw = true;

#if WITH_EDITORONLY_DATA
    UPROPERTY(EditAnywhere)
    bool bDrawDebug = false;
#endif

    UPROPERTY(EditAnywhere)
    float Density = 1.0f;

    UPROPERTY(EditAnywhere)
    FVector2D Size = FVector2D(4.0f, 4.0f);

protected:

    // ~ FSceneComponentTemplateDefinition
    virtual TSubclassOf<UActorComponent> GetComponentClass() const override;
    virtual void InitializeInstance(UActorComponent* Target) override;
    // ~ FSceneComponentTemplateDefinition

};

