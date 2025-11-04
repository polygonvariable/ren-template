// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Actor/WeatherManagerActor.h"

// Engine Headers

// Project Header
#include "RCoreAssetManager/Public/RAssetManager.h"
#include "RCoreAssetManager/Private/RAssetManager.inl"

#include "RCoreLibrary/Public/LogCategory.h"
#include "RCoreLibrary/Public/LogMacro.h"

#include "RenWeather/Public/Actor/WeatherEffectActor.h"
#include "RenWeather/Public/WeatherAsset.h"
#include "RenWeather/Public/WeatherSubsystem.h"



AWeatherManagerActor::AWeatherManagerActor()
{
    PrimaryActorTick.bCanEverTick = false;
}


void AWeatherManagerActor::HandleWeatherChanged(UPrimaryDataAsset* Asset)
{
    UWeatherAsset* WeatherAsset = Cast<UWeatherAsset>(Asset);
    if (!IsValid(AssetManager) || !IsValid(WeatherAsset) || LatentIds.Contains(WeatherAsset))
    {
        LOG_ERROR(LogWeather, TEXT("AssetManager, WeatherAsset is invalid or Weather is already active"));
		return;
    }

    FGuid LatentId = FGuid::NewGuid();
    LatentIds.Add(WeatherAsset, LatentId);

    const TArray<TSoftClassPtr<AWeatherEffectActor>>& Classes = WeatherAsset->EffectClasses;
    TFuture<FLatentResultAssets<UClass>> Future = AssetManager->FetchSecondaryClasses<AWeatherEffectActor>(LatentId, Classes);
    if (!Future.IsValid())
    {
		LOG_ERROR(LogWeather, TEXT("Future is invalid"));
		return;
    }

    TWeakObjectPtr<AWeatherManagerActor> WeakThis(this);
    TFunction<void(const FLatentResultAssets<UClass>&)> Callback = [WeakThis](const FLatentResultAssets<UClass>& Result)
        {
            AWeatherManagerActor* This = WeakThis.Get();
            if (IsValid(This) && Result.IsValid())
            {
                This->ActivateEffects(Result.Get());
            }
        };

    Future.Next(MoveTemp(Callback));
}

void AWeatherManagerActor::HandleWeatherRemoved(UPrimaryDataAsset* Asset)
{
    UWeatherAsset* WeatherAsset = Cast<UWeatherAsset>(Asset);
    if (!IsValid(AssetManager) || !IsValid(WeatherAsset) || !LatentIds.Contains(WeatherAsset))
    {
        LOG_ERROR(LogWeather, TEXT("AssetManager, WeatherAsset is invalid or Weather is not active"));
        return;
    }

    FGuid LatentId;
    LatentIds.RemoveAndCopyValue(WeatherAsset, LatentId);
    AssetManager->CancelFetch(LatentId);

    const TArray<TSoftClassPtr<AWeatherEffectActor>>& Classes = WeatherAsset->EffectClasses;
    DeactivateEffects(Classes);
}

void AWeatherManagerActor::ActivateEffects(const TArray<UClass*>& Classes)
{
    UWorld* World = GetWorld();

    FActorSpawnParameters SpawnParameters;
    SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    SpawnParameters.Owner = this;

    for (UClass* Class : Classes)
    {
        TObjectPtr<AWeatherEffectActor>* FoundActor = EffectActors.Find(Class);
        if (FoundActor)
        {
            AWeatherEffectActor* Actor = FoundActor->Get();
            if (IsValid(Actor))
            {
                Actor->ActivateEffect();
            }
            continue;
        }
        
        AWeatherEffectActor* NewActor = World->SpawnActor<AWeatherEffectActor>(Class, SpawnParameters);
        if (IsValid(NewActor))
        {
            NewActor->ActivateEffect();
            EffectActors.Add(Class, NewActor);
        }
    }
}

void AWeatherManagerActor::DeactivateEffects(const TArray<TSoftClassPtr<AWeatherEffectActor>>& Classes)
{
    for (const TSoftClassPtr<AWeatherEffectActor>& Class : Classes)
    {
        TObjectPtr<AWeatherEffectActor>* FoundActor = EffectActors.Find(Class.Get());
        if (!FoundActor)
        {
            continue;
        }

        AWeatherEffectActor* EffectActor = FoundActor->Get();
        if (IsValid(EffectActor))
        {
            EffectActor->DeactivateEffect();
        }
    }
}


void AWeatherManagerActor::BeginPlay()
{
    UWorld* World = GetWorld();
    UWeatherSubsystem* Weather = World->GetSubsystem<UWeatherSubsystem>();
    if (IsValid(Weather))
    {
        Weather->Delegates.OnChanged.AddUObject(this, &AWeatherManagerActor::HandleWeatherChanged);
        Weather->Delegates.OnRemoved.AddUObject(this, &AWeatherManagerActor::HandleWeatherRemoved);

        WeatherSubsystem = Weather;
    }

    AssetManager = Cast<URAssetManager>(UAssetManager::GetIfInitialized());

    Super::BeginPlay();
}

void AWeatherManagerActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    for (TPair<TSubclassOf<AWeatherEffectActor>, TObjectPtr<AWeatherEffectActor>>& EffectActor : EffectActors)
    {
        AWeatherEffectActor* Actor = EffectActor.Value.Get();
        if (IsValid(Actor))
        {
            Actor->Destroy();
        }
    }
    EffectActors.Empty();

    UWeatherSubsystem* Weather = WeatherSubsystem.Get();
    if (IsValid(Weather))
    {
        Weather->Delegates.RemoveAll(this);
    }
    WeatherSubsystem.Reset();

    AssetManager = nullptr;

    Super::EndPlay(EndPlayReason);
}

