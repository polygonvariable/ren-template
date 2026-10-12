// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Core/EnvironmentBrushComponentDefinition.h"

// Project Headers
#include "Component/EnvironmentBrushComponent.h"


TSubclassOf<UActorComponent> FEnvironmentBrushComponentDefinition::GetComponentClass() const
{
    return UEnvironmentBrushComponent::StaticClass();
}

void FEnvironmentBrushComponentDefinition::InitializeInstance(UActorComponent* Target)
{
    UEnvironmentBrushComponent* Component = Cast<UEnvironmentBrushComponent>(Target);
    checkf(::IsValid(Component), TEXT("Equipment brush template target is invalid or not of valid type"));

    Component->bLineTrace = bLineTrace;
    Component->bCanDraw = bCanDraw;
#if WITH_EDITOR
    Component->bDrawDebug = bDrawDebug;
#endif
    Component->BrushDensity = Density;
    Component->BrushSize = Size;
};

