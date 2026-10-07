// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Core/EnvironmentBrushTemplateData.h"

// Project Headers
#include "Component/EnvironmentBrushComponent.h"


TSubclassOf<UActorComponent> FEnvironmentBrushTemplateData::GetComponentClass() const
{
    return UEnvironmentBrushComponent::StaticClass();
}

bool FEnvironmentBrushTemplateData::ApplyToInstance(UObject* Target)
{
    UEnvironmentBrushComponent* Component = Cast<UEnvironmentBrushComponent>(Target);
    if (!::IsValid(Component))
    {
        return false;
    }
    Component->bLineTrace = bLineTrace;
    Component->bCanDraw = bCanDraw;
#if WITH_EDITOR
    Component->bDrawDebug = bDrawDebug;
#endif
    Component->BrushDensity = Density;
    Component->BrushSize = Size;
    return true;
};

