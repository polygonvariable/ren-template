// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Actor/WeatherEffectManagerActor.h"

// Engine Headers
#include "Engine/AssetManager.h"
#include "NiagaraSystem.h"

// Project Header
#include "Actor/WeatherEffectActor.h"
#include "Core/AssetManagerUtil.h"
#include "Data/WeatherAsset.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "System/WeatherController.h"
#include "System/WeatherSubsystem.h"


AWeatherEffectManagerActor::AWeatherEffectManagerActor()
{
    PrimaryActorTick.bCanEverTick = false;
    SetActorTickEnabled(false);
    SetActorEnableCollision(false);
    SetCanBeDamaged(false);
}


void AWeatherEffectManagerActor::ActivateEffects(const TArray<TSoftObjectPtr<UNiagaraSystem>>& Systems)
{
    UWorld* World = GetWorld();

    FActorSpawnParameters SpawnParameters;
    SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    SpawnParameters.Owner = this;

    for (const TSoftObjectPtr<UNiagaraSystem>& Item : Systems)
    {
        UNiagaraSystem* Niagara = Item.Get();
        if (!IsValid(Niagara))
        {
            continue;
        }

        TObjectPtr<AWeatherEffectActor>* FoundActor = EffectActors.FindByPredicate([Niagara](AWeatherEffectActor* Actor) { return IsValid(Actor) && Actor->GetNiagaraSystem() == Niagara; });
        if (FoundActor)
        {
            AWeatherEffectActor* Actor = FoundActor->Get();
            if (IsValid(Actor))
            {
                Actor->ActivateEffect();
            }
            continue;
        }

        AWeatherEffectActor* Actor = World->SpawnActorDeferred<AWeatherEffectActor>(AWeatherEffectActor::StaticClass(), FTransform(), this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
        if (IsValid(Actor))
        {
            Actor->SetNiagaraSystem(Niagara);
            Actor->ActivateEffect();
            Actor->FinishSpawning(FTransform());
            EffectActors.Add(Actor);
        }
    }
}

void AWeatherEffectManagerActor::DeactivateEffects(const TArray<TSoftObjectPtr<UNiagaraSystem>>& Systems)
{
    for (const TSoftObjectPtr<UNiagaraSystem>& Item : Systems)
    {
        UNiagaraSystem* Niagara = Item.Get();
        if (!IsValid(Niagara))
        {
            continue;
        }

        TObjectPtr<AWeatherEffectActor>* FoundActor = EffectActors.FindByPredicate([Niagara](AWeatherEffectActor* Actor) { return IsValid(Actor) && Actor->GetNiagaraSystem() == Niagara; });
        if (FoundActor)
        {
            AWeatherEffectActor* Actor = FoundActor->Get();
            if (IsValid(Actor))
            {
                Actor->DeactivateEffect();
            }
        }
    }
}

void AWeatherEffectManagerActor::RemoveLoadHandle(UWeatherAsset* WeatherAsset)
{
    TSharedPtr<FStreamableHandle> Handle;
    LoadHandles.RemoveAndCopyValue(WeatherAsset, Handle);
    FAssetManagerUtil::ReleaseHandle(Handle);
}


void AWeatherEffectManagerActor::HandleOnEffectLoaded(UWeatherAsset* WeatherAsset)
{
    RemoveLoadHandle(WeatherAsset);

    if (IsValid(WeatherAsset))
    {
        ActivateEffects(WeatherAsset->NiagaraSystems);
    }
}

void AWeatherEffectManagerActor::HandleWeatherChanged(UWeatherAsset* WeatherAsset)
{
    if (!IsValid(WeatherAsset) || LoadHandles.Contains(WeatherAsset))
    {
        LOG_ERROR(LogWeather, TEXT("AssetManager, WeatherAsset is invalid or Weather is already active"));
        return;
    }

    const TArray<TSoftObjectPtr<UNiagaraSystem>>& Systems = WeatherAsset->NiagaraSystems;
    if (Systems.Num() == 0)
    {
        LOG_ERROR(LogWeather, TEXT("Weather niagara systems is empty"));
        return;
    }

    TArray<FSoftObjectPath> SoftObjects;
    for (const TSoftObjectPtr<UNiagaraSystem>& Item : Systems)
    {
        SoftObjects.Add(Item.ToSoftObjectPath());
    }

    FStreamableManager& Manager = UAssetManager::GetStreamableManager();
    TSharedPtr<FStreamableHandle> Handle = Manager.RequestAsyncLoad(SoftObjects, FStreamableDelegate::CreateUObject(this, &AWeatherEffectManagerActor::HandleOnEffectLoaded, WeatherAsset));

    LoadHandles.Add(WeatherAsset, Handle);
}

void AWeatherEffectManagerActor::HandleWeatherRemoved(UWeatherAsset* WeatherAsset)
{
    if (!IsValid(WeatherAsset))
    {
        LOG_ERROR(LogWeather, TEXT("AssetManager, WeatherAsset is invalid or Weather is not active"));
        return;
    }

    RemoveLoadHandle(WeatherAsset);
    DeactivateEffects(WeatherAsset->NiagaraSystems);
}


void AWeatherEffectManagerActor::BeginPlay()
{
    WeatherSubsystem = GetWorld()->GetSubsystem<UWeatherSubsystem>();
    if (IsValid(WeatherSubsystem))
    {
        UWeatherController* WeatherController = WeatherSubsystem->GetWeatherController();
        if (IsValid(WeatherController))
		{
            WeatherController->OnWeatherChanged.AddUObject(this, &AWeatherEffectManagerActor::HandleWeatherChanged);
            WeatherController->OnWeatherRemoved.AddUObject(this, &AWeatherEffectManagerActor::HandleWeatherRemoved);
		}
    }

    Super::BeginPlay();
}

void AWeatherEffectManagerActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    for (TPair<TObjectPtr<UWeatherAsset>, TSharedPtr<FStreamableHandle>>& Kv : LoadHandles)
    {
        FAssetManagerUtil::CancelHandle(Kv.Value);
    }
    LoadHandles.Empty();

    for (TObjectPtr<AWeatherEffectActor> Item : EffectActors)
    {
        if (IsValid(Item))
        {
            Item->Destroy();
        }
    }
    EffectActors.Empty();

    if (IsValid(WeatherSubsystem))
    {
        UWeatherController* WeatherController = WeatherSubsystem->GetWeatherController();
        if (IsValid(WeatherController))
		{
            WeatherController->OnWeatherChanged.RemoveAll(this);
            WeatherController->OnWeatherRemoved.RemoveAll(this);
		}
    }
    WeatherSubsystem = nullptr;

    Super::EndPlay(EndPlayReason);
}

