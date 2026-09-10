// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Actor/WeatherEffectManagerActor.h"

// Engine Headers
#include "Engine/AssetManager.h"

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


void AWeatherEffectManagerActor::ActivateEffects(const TArray<TSoftClassPtr<AWeatherEffectActor>>& Classes)
{
    UWorld* World = GetWorld();

    FActorSpawnParameters SpawnParameters;
    SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    SpawnParameters.Owner = this;

    for (const TSoftClassPtr<AWeatherEffectActor>& Class : Classes)
    {
        UClass* EffectClass = Class.Get();
        if (!IsValid(EffectClass))
        {
            continue;
        }

        TObjectPtr<AWeatherEffectActor>* FoundActor = EffectActors.FindByPredicate([EffectClass](AWeatherEffectActor* Actor) { return IsValid(Actor) && Actor->IsA(EffectClass); });
        if (FoundActor)
        {
            AWeatherEffectActor* Actor = FoundActor->Get();
            if (IsValid(Actor))
            {
                Actor->ActivateEffect();
            }
            continue;
        }

        AWeatherEffectActor* Actor = World->SpawnActor<AWeatherEffectActor>(EffectClass, SpawnParameters);
        if (IsValid(Actor))
        {
            Actor->ActivateEffect();
            EffectActors.Add(Actor);
        }
    }
}

void AWeatherEffectManagerActor::DeactivateEffects(const TArray<TSoftClassPtr<AWeatherEffectActor>>& Classes)
{
    for (const TSoftClassPtr<AWeatherEffectActor>& Class : Classes)
    {
        UClass* EffectClass = Class.Get();
        if (!IsValid(EffectClass))
        {
            continue;
        }

        TObjectPtr<AWeatherEffectActor>* FoundActor = EffectActors.FindByPredicate([EffectClass](AWeatherEffectActor* Actor) { return IsValid(Actor) && Actor->IsA(EffectClass); });
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
        ActivateEffects(WeatherAsset->EffectClasses);
    }
}

void AWeatherEffectManagerActor::HandleWeatherChanged(UWeatherAsset* WeatherAsset)
{
    if (!IsValid(WeatherAsset) || LoadHandles.Contains(WeatherAsset))
    {
        LOG_ERROR(LogWeather, TEXT("AssetManager, WeatherAsset is invalid or Weather is already active"));
        return;
    }

    const TArray<TSoftClassPtr<AWeatherEffectActor>>& Classes = WeatherAsset->EffectClasses;
    if (Classes.Num() == 0)
    {
        LOG_ERROR(LogWeather, TEXT("Weather effect classes is empty"));
        return;
    }

    TArray<FSoftObjectPath> SoftObjects;
    for (const TSoftClassPtr<AWeatherEffectActor>& Class : Classes)
    {
        SoftObjects.Add(Class.ToSoftObjectPath());
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
    DeactivateEffects(WeatherAsset->EffectClasses);
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

