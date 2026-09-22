// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Core/Type/WeatherReactiveEffect.h"


void FWeatherReactiveEffect::Execute(UActorComponent* Component, bool bIsAdded)
{
    SetAlpha(Component, bIsAdded ? 1.0f : 0.0f);
}

void FWeatherReactiveEffect::SetAlpha(UActorComponent* Component, float NewAlpha)
{
    NewAlpha = FMath::Clamp(NewAlpha, 0.0f, 1.0f);
    if (!FMath::IsNearlyEqual(CurrentAlpha, NewAlpha, KINDA_SMALL_NUMBER))
    {
        OnApplyChange(Component, CurrentAlpha, NewAlpha);
        CurrentAlpha = NewAlpha;
    }
}


void FWeatherReactiveEffect_MoveComponent::OnApplyChange(UActorComponent* Component, float OldAlpha, float NewAlpha)
{
    USceneComponent* Scene = Cast<USceneComponent>(Component);
    if (IsValid(Scene))
    {
        Scene->AddLocalOffset(Offset * (NewAlpha - OldAlpha));
    }
}

