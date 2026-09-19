// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Component/WeatherWorldEffectComponent.h"

// Engine Headers
#include "UObject/ObjectSaveContext.h"
#include "Misc/DataValidation.h"

// Project Headers
#include "Core/Type/WeatherWorldEffect.h"
#include "Data/WeatherAsset.h"
#include "Log/LogMacro.h"
#include "System/WeatherController.h"
#include "System/WeatherSubsystem.h"


UWeatherWorldEffectComponent::UWeatherWorldEffectComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
}


void UWeatherWorldEffectComponent::BeginPlay()
{
    WeatherSubsystem = UWeatherSubsystem::Get(GetWorld());
    if (IsValid(WeatherSubsystem))
    {
        WeatherSubsystem->OnControllerCreated.AddUObject(this, &UWeatherWorldEffectComponent::HandleOnControllerCreated);
    }

    Super::BeginPlay();
}

void UWeatherWorldEffectComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    CancelAllTransition();

    if (IsValid(WeatherSubsystem))
    {
        WeatherSubsystem->OnWeatherRefreshed.RemoveAll(this);
    }

    if (IsValid(WeatherController))
    {
        WeatherController->OnWeatherChanged.RemoveAll(this);
        WeatherController->OnWeatherRemoved.RemoveAll(this);
    }
    WeatherController = nullptr;

    Super::EndPlay(EndPlayReason);
}


void UWeatherWorldEffectComponent::PreSave(FObjectPreSaveContext SaveContext)
{
    Super::PreSave(SaveContext);

#if WITH_EDITOR
    for (FInstancedStruct& Item : WorldEffects)
    {
        FWeatherWorldEffect* Effect = Item.GetMutablePtr<FWeatherWorldEffect>();
        if (Effect)
        {
            Effect->CurrentAlpha = 0.0f;
        }
    }
#endif
}

#if WITH_EDITOR
EDataValidationResult UWeatherWorldEffectComponent::IsDataValid(FDataValidationContext& Context) const
{
    EDataValidationResult Result = EDataValidationResult::Valid;

    for (const FInstancedStruct& Item : WorldEffects)
    {
        const FWeatherWorldEffect* Effect = Item.GetPtr<FWeatherWorldEffect>();
        if (!Effect)
        {
            continue;
        }
        
        AActor* Owner = GetOwner();
        UActorComponent* Component = Owner->FindComponentByTag<UActorComponent>(Effect->ComponentTag);
        if (!IsValid(Component))
        {
            Context.AddError(FText::FromString(TEXT("Component not found")));
            return EDataValidationResult::Invalid;
        }

        if (Effect->bCanTransition && Effect->TransitionDuration < 1.0f)
        {
            Context.AddError(FText::FromString(TEXT("Transition duration is invalid")));
            return EDataValidationResult::Invalid;
        }
    }

    if (TransitionRate < 0.1f)
    {
        Context.AddError(FText::FromString(TEXT("Transition rate is invalid")));
        return EDataValidationResult::Invalid;
    }

    return Result;
}
#endif


void UWeatherWorldEffectComponent::HandleOnControllerCreated()
{
    if (IsValid(WeatherSubsystem))
    {
        WeatherSubsystem->OnControllerCreated.RemoveAll(this);

        WeatherController = WeatherSubsystem->GetWeatherController();
        if (IsValid(WeatherController))
        {
            WeatherController->OnWeatherChanged.AddUObject(this, &UWeatherWorldEffectComponent::HandleOnWeatherChanged);
            WeatherController->OnWeatherRemoved.AddUObject(this, &UWeatherWorldEffectComponent::HandleOnWeatherRemoved);
        }
    }
}

void UWeatherWorldEffectComponent::HandleOnWeatherChanged(UWeatherAsset* WeatherAsset)
{
    if (!IsValid(WeatherAsset))
    {
        return;
    }

    const FPrimaryAssetId AssetId = WeatherAsset->GetPrimaryAssetId();

    int Num = WorldEffects.Num();
    for (int i = 0; i < Num; ++i)
    {
        FWeatherWorldEffect* Effect = WorldEffects[i].GetMutablePtr<FWeatherWorldEffect>();
        if (!Effect || Effect->WeatherId != AssetId)
        {
            continue;
        }

        AActor* Owner = GetOwner();
        if (!IsValid(Owner))
        {
            continue;
        }

        UActorComponent* Component = Owner->FindComponentByTag<UActorComponent>(Effect->ComponentTag);
        if (!IsValid(Component))
        {
            continue;
        }

        if (!Effect->bCanTransition)
        {
            Effect->Execute(Component, true);
            continue;
        }

        StartTransition(i, Component, true);
    }
}

void UWeatherWorldEffectComponent::HandleOnWeatherRemoved(UWeatherAsset* WeatherAsset)
{
    if (!IsValid(WeatherAsset))
    {
        return;
    }

    const FPrimaryAssetId AssetId = WeatherAsset->GetPrimaryAssetId();

    int Num = WorldEffects.Num();
    for (int i = 0; i < Num; ++i)
    {
        FWeatherWorldEffect* Effect = WorldEffects[i].GetMutablePtr<FWeatherWorldEffect>();
        if (!Effect || Effect->WeatherId != AssetId)
        {
            continue;
        }

        AActor* Owner = GetOwner();
        if (!IsValid(Owner))
        {
            continue;
        }

        UActorComponent* Component = Owner->FindComponentByTag<UActorComponent>(Effect->ComponentTag);
        if (!IsValid(Component))
        {
            StopTransition(i);
            Effect->CurrentAlpha = 0.0f;
            continue;
        }

        if (!Effect->bCanTransition)
        {
            Effect->Execute(Component, false);
            continue;
        }

        const bool bHasTransition = ActiveTransitions.ContainsByPredicate([i](const FWeatherEffectTransitionHandle& Transition) { return Transition.EffectIndex == i; });
        if (!bHasTransition && FMath::IsNearlyZero(Effect->CurrentAlpha))
        {
            continue;
        }

        StartTransition(i, Component, false);
    }
}


void UWeatherWorldEffectComponent::StartTransition(int EffectIndex, UActorComponent* Component, bool bIsAdded)
{
    if (!WorldEffects.IsValidIndex(EffectIndex) || !IsValid(Component))
    {
        return;
    }

    FWeatherWorldEffect* Effect = WorldEffects[EffectIndex].GetMutablePtr<FWeatherWorldEffect>();
    if (!Effect)
    {
        return;
    }

    const float TargetAlpha = bIsAdded ? 1.0f : 0.0f;
    if (FMath::IsNearlyEqual(Effect->CurrentAlpha, TargetAlpha, KINDA_SMALL_NUMBER))
    {
        StopTransition(EffectIndex);
        return;
    }

    const float Duration = FMath::Max(Effect->TransitionDuration, KINDA_SMALL_NUMBER);

    FWeatherEffectTransitionHandle* FoundTransition = ActiveTransitions.FindByPredicate([EffectIndex](const FWeatherEffectTransitionHandle& Transition) { return Transition.EffectIndex == EffectIndex; });
    if (FoundTransition)
    {
        FoundTransition->TargetComponent = Component;
        FoundTransition->Duration = Duration;
        FoundTransition->Elapsed = 0.0f;
        FoundTransition->StartAlpha = Effect->CurrentAlpha;
        FoundTransition->TargetAlpha = TargetAlpha;
    }
    else
    {
        FWeatherEffectTransitionHandle NewTransition;
        NewTransition.EffectIndex = EffectIndex;
        NewTransition.TargetComponent = Component;
        NewTransition.Duration = Duration;
        NewTransition.Elapsed = 0.0f;
        NewTransition.StartAlpha = Effect->CurrentAlpha;
        NewTransition.TargetAlpha = TargetAlpha;
        ActiveTransitions.Add(MoveTemp(NewTransition));
    }

    UWorld* World = GetWorld();
    if (IsValid(World))
    {
        FTimerManager& TimerManager = World->GetTimerManager();
        if (!TimerManager.IsTimerActive(TransitionTimer))
        {
            PRINT_WARNING(LogTemp, 1.0f, TEXT("Weather effect started"));
            TimerManager.SetTimer(TransitionTimer, this, &UWeatherWorldEffectComponent::HandleOnTransitionTick, FMath::Max(TransitionRate, 0.1f), true);
        }
    }
}

void UWeatherWorldEffectComponent::StopTransition(int EffectIndex)
{
    ActiveTransitions.RemoveAll([EffectIndex](const FWeatherEffectTransitionHandle& Transition) { return Transition.EffectIndex == EffectIndex; });
}

void UWeatherWorldEffectComponent::CancelAllTransition()
{
    ActiveTransitions.Reset();

    UWorld* World = GetWorld();
    if (IsValid(World))
    {
        PRINT_WARNING(LogTemp, 1.0f, TEXT("Weather effect stopped"));
        
        FTimerManager& TimerManager = World->GetTimerManager();
        TimerManager.ClearTimer(TransitionTimer);
    }
}

void UWeatherWorldEffectComponent::HandleOnTransitionTick()
{
    UWorld* World = GetWorld();
    if (!IsValid(World))
    {
        CancelAllTransition();
        return;
    }

    const float Rate = FMath::Max(TransitionRate, 0.1f);

    for (int i = ActiveTransitions.Num() - 1; i >= 0; --i)
    {
        FWeatherEffectTransitionHandle& Transition = ActiveTransitions[i];
        if (!Transition.TargetComponent.IsValid() || !WorldEffects.IsValidIndex(Transition.EffectIndex))
        {
            ActiveTransitions.RemoveAt(i);
            continue;
        }

        FWeatherWorldEffect* Effect = WorldEffects[Transition.EffectIndex].GetMutablePtr<FWeatherWorldEffect>();
        if (!Effect)
        {
            ActiveTransitions.RemoveAt(i);
            continue;
        }

        Transition.Elapsed += Rate;

        const float NewAlpha = FMath::Clamp(Transition.Elapsed / FMath::Max(Transition.Duration, KINDA_SMALL_NUMBER), 0.0f, 1.0f);
        const float EasedAlpha = FMath::SmoothStep(0.0f, 1.0f, NewAlpha);
        const float FinalAlpha = FMath::Lerp(Transition.StartAlpha, Transition.TargetAlpha, EasedAlpha);

        Effect->SetAlpha(Transition.TargetComponent.Get(), FinalAlpha);

        if (NewAlpha >= 1.0f)
        {
            ActiveTransitions.RemoveAt(i);
        }
    }

    if (ActiveTransitions.Num() == 0)
    {
        PRINT_WARNING(LogTemp, 1.0f, TEXT("Weather effect stopped"));

        FTimerManager& TimerManager = World->GetTimerManager();
        TimerManager.ClearTimer(TransitionTimer);
    }
}

