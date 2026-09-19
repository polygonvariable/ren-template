// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Actor/WeatherEffectActor.h"

// Engine Headers
#include "Components/ArrowComponent.h"
#include "Components/AudioComponent.h"
#include "NiagaraComponent.h"


AWeatherEffectActor::AWeatherEffectActor()
{
    SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

    if (IsValid(SceneComponent))
    {
        SetRootComponent(SceneComponent);

#if WITH_EDITORONLY_DATA
        ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
        if (IsValid(ArrowComponent))
        {
            ArrowComponent->SetupAttachment(SceneComponent);
            ArrowComponent->ArrowSize = 4.0f;
            ArrowComponent->ArrowLength = 30.0f;
            //ArrowComponent->bHiddenInGame = false;
        }
#endif
    }
    
    PrimaryActorTick.bCanEverTick = false;
    SetActorEnableCollision(false);
    SetCanBeDamaged(false);
}


bool AWeatherEffectActor::InitializeEffect()
{
    return false;
}

void AWeatherEffectActor::DeinitializeEffect()
{
    EffectPath.Reset();
    RemoveFollow();
}


void AWeatherEffectActor::ActivateEffect()
{
    CreateFollow();
}

void AWeatherEffectActor::DeactivateEffect()
{
    RemoveFollow();
}


#if UE_BUILD_DEVELOPMENT
UActorComponent* AWeatherEffectActor::GetEditorEffectComponent() const
{
    return nullptr;
}
#endif


void AWeatherEffectActor::CreateFollow()
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

    AttachToActor(Pawn, FAttachmentTransformRules::KeepRelativeTransform);

    //RemoveFollowTimer();
    //FTimerManager& TimerManager = GetWorld()->GetTimerManager();
    //TimerManager.SetTimer(FollowTimer, this, &AWeatherEffectActor::HandleOnTimerTick, 2.5f, FTimerManagerTimerParameters{ .bLoop = true, .bMaxOncePerFrame = true });
}

void AWeatherEffectActor::RemoveFollow()
{
    DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
    //FTimerManager& TimerManager = GetWorld()->GetTimerManager();
    //TimerManager.ClearTimer(FollowTimer);
    //FollowTimer.Invalidate();
}

void AWeatherEffectActor::HandleOnTimerTick()
{
    //APlayerController* Controller = GetWorld()->GetFirstPlayerController();
    //if (!IsValid(Controller))
    //{
    //    return;
    //}

    //APawn* Pawn = Controller->GetPawn();
    //if (!IsValid(Pawn))
    //{
    //    return;
    //}

    //SetActorLocation(Pawn->GetActorLocation());
}

void AWeatherEffectActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    DeactivateEffect();

    Super::EndPlay(EndPlayReason);
}



AWeatherEffectParticle::AWeatherEffectParticle() : Super()
{
    NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
    if (IsValid(NiagaraComponent))
    {
        NiagaraComponent->SetupAttachment(SceneComponent);
        NiagaraComponent->bAutoActivate = false;
    }
}

bool AWeatherEffectParticle::InitializeEffect()
{
    UNiagaraSystem* Asset = Cast<UNiagaraSystem>(EffectPath.ResolveObject());
    if (!IsValid(NiagaraComponent) || !IsValid(Asset))
    {
        return false;
    }
    NiagaraComponent->SetAsset(Asset);
    return true;
}

void AWeatherEffectParticle::ActivateEffect()
{
    Super::ActivateEffect();
    if (IsValid(NiagaraComponent))
    {
        NiagaraComponent->Activate(true);
    }
}

void AWeatherEffectParticle::DeactivateEffect()
{
    if (IsValid(NiagaraComponent))
    {
        NiagaraComponent->Deactivate();
    }
    Super::DeactivateEffect();
}

#if UE_BUILD_DEVELOPMENT
UActorComponent* AWeatherEffectParticle::GetEditorEffectComponent() const
{
    return NiagaraComponent;
}
#endif



AWeatherEffectAudio::AWeatherEffectAudio() : Super()
{
    AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
    if (IsValid(AudioComponent))
    {
        AudioComponent->SetupAttachment(RootComponent);
        AudioComponent->bAutoActivate = false;
    }
}

bool AWeatherEffectAudio::InitializeEffect()
{
    USoundBase* Asset = Cast<USoundBase>(EffectPath.ResolveObject());
    if (!IsValid(AudioComponent) || !IsValid(Asset))
    {
        return false;
    }
    AudioComponent->SetSound(Asset);
    return true;
}

void AWeatherEffectAudio::ActivateEffect()
{
    Super::ActivateEffect();
    if (IsValid(AudioComponent))
    {
        AudioComponent->FadeIn(2.0f, 1.0f);
    }
}

void AWeatherEffectAudio::DeactivateEffect()
{
    if (IsValid(AudioComponent))
    {
        AudioComponent->FadeOut(2.0f, 0.0f);
    }
    Super::DeactivateEffect();
}

#if UE_BUILD_DEVELOPMENT
UActorComponent* AWeatherEffectAudio::GetEditorEffectComponent() const
{
    return AudioComponent;
}
#endif
