// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Actor/WeatherEffectActor.h"

// Engine Headers
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif
#include "Components/ArrowComponent.h"
#include "NiagaraComponent.h"


AWeatherEffectActor::AWeatherEffectActor()
{
    SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

    if (IsValid(SceneComponent))
    {
        SetRootComponent(SceneComponent);

        NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
        if (IsValid(NiagaraComponent))
        {
            NiagaraComponent->SetupAttachment(SceneComponent);
            NiagaraComponent->bAutoActivate = false;
        }

#if WITH_EDITORONLY_DATA
        ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
        if (IsValid(ArrowComponent))
        {
            ArrowComponent->SetupAttachment(SceneComponent);
            ArrowComponent->ArrowSize = 4.0f;
            ArrowComponent->ArrowLength = 30.0f;
            ArrowComponent->bHiddenInGame = false;
        }
#endif
    }

    PrimaryActorTick.bCanEverTick = false;
    SetActorEnableCollision(false);
    SetCanBeDamaged(false);
}


void AWeatherEffectActor::SetNiagaraSystem(UNiagaraSystem* Asset)
{
    if (IsValid(NiagaraComponent) && IsValid(Asset))
    {
        NiagaraComponent->SetAsset(Asset);
    }
}

UNiagaraSystem* AWeatherEffectActor::GetNiagaraSystem() const
{
    if (!IsValid(NiagaraComponent))
    {
        return nullptr;
    }
    return NiagaraComponent->GetAsset();
}


void AWeatherEffectActor::ActivateEffect()
{
    if (IsValid(NiagaraComponent))
    {
        NiagaraComponent->Activate(true);
        CreateFollowTimer();
    }
}

void AWeatherEffectActor::DeactivateEffect()
{
    if (IsValid(NiagaraComponent))
    {
        NiagaraComponent->Deactivate();
    }
    RemoveFollowTimer();
}


#if WITH_EDITOR
UNiagaraComponent* AWeatherEffectActor::GetEditorNiagaraComponent()
{
    return NiagaraComponent;
}

EDataValidationResult AWeatherEffectActor::IsDataValid(FDataValidationContext& Context) const
{
    EDataValidationResult Result = Super::IsDataValid(Context);

    if (IsValid(NiagaraComponent))
    {
        UNiagaraSystem* Asset = NiagaraComponent->GetAsset();
        if (!IsValid(Asset))
        {
            Context.AddError(FText::FromString("Niagara system is invalid"));
            Result = EDataValidationResult::Invalid;
        }
    }

    return Result;
}
#endif


void AWeatherEffectActor::CreateFollowTimer()
{
    RemoveFollowTimer();

    FTimerManager& TimerManager = GetWorld()->GetTimerManager();
    TimerManager.SetTimer(FollowTimer, this, &AWeatherEffectActor::HandleOnTimerTick, 2.5f, FTimerManagerTimerParameters{ .bLoop = true, .bMaxOncePerFrame = true });
}

void AWeatherEffectActor::RemoveFollowTimer()
{
    FTimerManager& TimerManager = GetWorld()->GetTimerManager();
    TimerManager.ClearTimer(FollowTimer);
    FollowTimer.Invalidate();
}

void AWeatherEffectActor::HandleOnTimerTick()
{
    APlayerController* Controller = GetWorld()->GetFirstPlayerController();
    if (!IsValid(Controller))
    {
        return;
    }

    APawn* Pawn = Controller->GetPawn();
    if (!IsValid(Pawn))
    {
        return;
    }

    SetActorLocation(Pawn->GetActorLocation());
}


void AWeatherEffectActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    DeactivateEffect();

    Super::EndPlay(EndPlayReason);
}

