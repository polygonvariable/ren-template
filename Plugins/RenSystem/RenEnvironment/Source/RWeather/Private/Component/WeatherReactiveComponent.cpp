// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Component/WeatherReactiveComponent.h"

// Engine Headers
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif
#include "UObject/ObjectSaveContext.h"

// Project Headers
#include "Core/Type/WeatherReactiveEffect.h"
#include "Data/WeatherAsset.h"
#include "Log/LogMacro.h"
#include "System/WeatherController.h"
#include "System/WeatherSubsystem.h"


UWeatherReactiveComponent::UWeatherReactiveComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
    bWantsInitializeComponent = true;
}


void UWeatherReactiveComponent::InitializeComponent()
{
    Super::InitializeComponent();

    WeatherSubsystem = UWeatherSubsystem::Get(GetWorld());
    if (IsValid(WeatherSubsystem))
    {
        if (IsValid(WeatherSubsystem->GetWeatherController()))
        {
            HandleOnControllerCreated();
            return;
        }

        WeatherSubsystem->OnControllerCreated.AddUObject(this, &UWeatherReactiveComponent::HandleOnControllerCreated);
    }
}

void UWeatherReactiveComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    CancelAllTransition();

    if (IsValid(WeatherSubsystem))
    {
        WeatherSubsystem->OnWeatherRefreshed.RemoveAll(this);
    }
    WeatherSubsystem = nullptr;

    if (IsValid(WeatherController))
    {
        WeatherController->OnWeatherChanged.RemoveAll(this);
        WeatherController->OnWeatherRemoved.RemoveAll(this);
    }
    WeatherController = nullptr;

    Super::EndPlay(EndPlayReason);
}


void UWeatherReactiveComponent::PreSave(FObjectPreSaveContext SaveContext)
{
    Super::PreSave(SaveContext);

#if WITH_EDITOR
    for (FInstancedStruct& Item : WorldEffects)
    {
        FWeatherReactiveEffect* Effect = Item.GetMutablePtr<FWeatherReactiveEffect>();
        if (Effect)
        {
            Effect->CurrentAlpha = 0.0f;
        }
    }
#endif
}

#if WITH_EDITOR
EDataValidationResult UWeatherReactiveComponent::IsDataValid(FDataValidationContext& Context) const
{
    EDataValidationResult Result = EDataValidationResult::Valid;

    for (const FInstancedStruct& Item : WorldEffects)
    {
        const FWeatherReactiveEffect* Effect = Item.GetPtr<FWeatherReactiveEffect>();
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


void UWeatherReactiveComponent::HandleOnControllerCreated()
{
    if (IsValid(WeatherSubsystem))
    {
        WeatherSubsystem->OnControllerCreated.RemoveAll(this);

        WeatherController = WeatherSubsystem->GetWeatherController();
        if (IsValid(WeatherController))
        {
            WeatherController->OnWeatherChanged.AddUObject(this, &UWeatherReactiveComponent::HandleOnWeatherChanged);
            WeatherController->OnWeatherRemoved.AddUObject(this, &UWeatherReactiveComponent::HandleOnWeatherRemoved);
        }
    }
}

void UWeatherReactiveComponent::HandleOnWeatherChanged(UWeatherAsset* WeatherAsset)
{
    if (!IsValid(WeatherAsset))
    {
        return;
    }

    const FPrimaryAssetId AssetId = WeatherAsset->GetPrimaryAssetId();

    int Num = WorldEffects.Num();
    for (int i = 0; i < Num; ++i)
    {
        FWeatherReactiveEffect* Effect = WorldEffects[i].GetMutablePtr<FWeatherReactiveEffect>();
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

void UWeatherReactiveComponent::HandleOnWeatherRemoved(UWeatherAsset* WeatherAsset)
{
    if (!IsValid(WeatherAsset))
    {
        return;
    }

    const FPrimaryAssetId AssetId = WeatherAsset->GetPrimaryAssetId();

    int Num = WorldEffects.Num();
    for (int i = 0; i < Num; ++i)
    {
        FWeatherReactiveEffect* Effect = WorldEffects[i].GetMutablePtr<FWeatherReactiveEffect>();
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


void UWeatherReactiveComponent::StartTransition(int EffectIndex, UActorComponent* Component, bool bIsAdded)
{
    if (!WorldEffects.IsValidIndex(EffectIndex) || !IsValid(Component))
    {
        return;
    }

    FWeatherReactiveEffect* Effect = WorldEffects[EffectIndex].GetMutablePtr<FWeatherReactiveEffect>();
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
            TimerManager.SetTimer(TransitionTimer, this, &UWeatherReactiveComponent::HandleOnTransitionTick, FMath::Max(TransitionRate, 0.1f), true);
        }
    }
}

void UWeatherReactiveComponent::StopTransition(int EffectIndex)
{
    ActiveTransitions.RemoveAll([EffectIndex](const FWeatherEffectTransitionHandle& Transition) { return Transition.EffectIndex == EffectIndex; });
}

void UWeatherReactiveComponent::CancelAllTransition()
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

void UWeatherReactiveComponent::HandleOnTransitionTick()
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

        FWeatherReactiveEffect* Effect = WorldEffects[Transition.EffectIndex].GetMutablePtr<FWeatherReactiveEffect>();
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

