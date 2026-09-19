// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Actor/WeatherEffectManager.h"

// Engine Headers
#include "Engine/AssetManager.h"
#include "NiagaraComponent.h"

// Project Header
#include "Actor/WeatherEffectActor.h"
#include "Core/AssetManagerUtil.h"
#include "Core/WeatherSettings.h"
#include "Data/WeatherAsset.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "System/WeatherController.h"
#include "System/WeatherSubsystem.h"


AWeatherEffectManager::AWeatherEffectManager()
{
    PrimaryActorTick.bCanEverTick = false;
    SetActorTickEnabled(false);
    SetActorEnableCollision(false);
    SetCanBeDamaged(false);
}

#if UE_BUILD_DEVELOPMENT
const TMap<TObjectPtr<UWeatherAsset>, TSharedPtr<FStreamableHandle>>& AWeatherEffectManager::GetEditorWeatherEffectHandles() const
{
    return LoadHandles;
}

const TArray<TObjectPtr<AWeatherEffectActor>>& AWeatherEffectManager::GetEditorWeatherEffects() const
{
    return EffectActors;
}
#endif

UClass* AWeatherEffectManager::GetEffectActorClass(UObject* Object) const
{
    if (!IsValid(Object))
    {
        return nullptr;
    }

    UClass* EffectClass = nullptr;
    const UWeatherSettings* Settings = UWeatherSettings::Get();

    if (Object->IsA<UNiagaraSystem>())
    {
        const TSubclassOf<AWeatherEffectActor>* FoundClass = Settings->EffectActors.Find(UNiagaraSystem::StaticClass());
        if (!FoundClass)
        {
            return nullptr;
        }
        return *FoundClass;
    }
    else if (Object->IsA<USoundBase>())
    {
        const TSubclassOf<AWeatherEffectActor>* FoundClass = Settings->EffectActors.Find(USoundBase::StaticClass());
        if (!FoundClass)
        {
            return nullptr;
        }
        return *FoundClass;
    }

    return nullptr;
}

void AWeatherEffectManager::ActivateEffects(const TArray<FSoftObjectPath>& Effects)
{
    UWorld* World = GetWorld();

    FActorSpawnParameters SpawnParameters;
    SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    SpawnParameters.Owner = this;

    for (const FSoftObjectPath& EffectPath : Effects)
    {
        if (!EffectPath.IsValid())
        {
            continue;
        }

        TObjectPtr<AWeatherEffectActor>* FoundActor = EffectActors.FindByPredicate([EffectPath](AWeatherEffectActor* Actor) { return IsValid(Actor) && Actor->EffectPath == EffectPath; });
        if (FoundActor)
        {
            AWeatherEffectActor* Actor = FoundActor->Get();
            if (IsValid(Actor))
            {
                Actor->ActivateEffect();
            }
            continue;
        }

        UClass* EffectClass = GetEffectActorClass(EffectPath.ResolveObject());
        if (!IsValid(EffectClass))
        {
            continue;
        }

        AWeatherEffectActor* Actor = World->SpawnActorDeferred<AWeatherEffectActor>(EffectClass, FTransform(), this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
        if (IsValid(Actor))
        {
            Actor->EffectPath = EffectPath;
            if (Actor->InitializeEffect())
            {
                Actor->ActivateEffect();
                Actor->FinishSpawning(FTransform());
                EffectActors.Add(Actor);
            }
        }
    }
}

void AWeatherEffectManager::DeactivateEffects(const TArray<FSoftObjectPath>& Effects)
{
    for (const FSoftObjectPath& Effect : Effects)
    {
        if (!Effect.IsValid())
        {
            continue;
        }

        TObjectPtr<AWeatherEffectActor>* FoundActor = EffectActors.FindByPredicate([Effect](AWeatherEffectActor* Actor) { return IsValid(Actor) && Actor->EffectPath == Effect; });
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

void AWeatherEffectManager::DeactivateAllEffects()
{
    for (TObjectPtr<AWeatherEffectActor> Item : EffectActors)
    {
        AWeatherEffectActor* Actor = Item.Get();
        if (IsValid(Actor))
        {
            Actor->DeactivateEffect();
        }
    }
}

void AWeatherEffectManager::RemoveLoadHandle(UWeatherAsset* WeatherAsset)
{
    TSharedPtr<FStreamableHandle> Handle;
    LoadHandles.RemoveAndCopyValue(WeatherAsset, Handle);
    FAssetManagerUtil::CancelHandle(Handle);
}


void AWeatherEffectManager::HandleOnEffectLoaded(UWeatherAsset* WeatherAsset)
{
    RemoveLoadHandle(WeatherAsset);

    if (IsValid(WeatherAsset))
    {
        ActivateEffects(WeatherAsset->WeatherEffects);
    }
}

void AWeatherEffectManager::HandleWeatherChanged(UWeatherAsset* WeatherAsset)
{
    DeactivateAllEffects();

    if (!IsValid(WeatherAsset) || LoadHandles.Contains(WeatherAsset))
    {
        LOG_ERROR(LogWeather, TEXT("AssetManager, WeatherAsset is invalid or Weather is already active"));
        return;
    }

    const TArray<FSoftObjectPath>& SoftObjects = WeatherAsset->WeatherEffects;
    if (SoftObjects.Num() == 0)
    {
        LOG_ERROR(LogWeather, TEXT("No objects to load"));
        return;
    }

    FStreamableManager& Manager = UAssetManager::GetStreamableManager();
    TSharedPtr<FStreamableHandle> Handle = Manager.RequestAsyncLoad(SoftObjects, FStreamableDelegate::CreateUObject(this, &AWeatherEffectManager::HandleOnEffectLoaded, WeatherAsset));

    LoadHandles.Add(WeatherAsset, Handle);
}

void AWeatherEffectManager::HandleWeatherRemoved(UWeatherAsset* WeatherAsset)
{
    if (!IsValid(WeatherAsset))
    {
        LOG_ERROR(LogWeather, TEXT("AssetManager, WeatherAsset is invalid or Weather is not active"));
        return;
    }

    RemoveLoadHandle(WeatherAsset);
    DeactivateEffects(WeatherAsset->WeatherEffects);
}


void AWeatherEffectManager::BeginPlay()
{
    WeatherSubsystem = GetWorld()->GetSubsystem<UWeatherSubsystem>();
    if (IsValid(WeatherSubsystem))
    {
        UWeatherController* WeatherController = WeatherSubsystem->GetWeatherController();
        if (IsValid(WeatherController))
		{
            WeatherController->OnWeatherChanged.AddUObject(this, &AWeatherEffectManager::HandleWeatherChanged);
            WeatherController->OnWeatherRemoved.AddUObject(this, &AWeatherEffectManager::HandleWeatherRemoved);
		}
    }

    Super::BeginPlay();
}

void AWeatherEffectManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    for (TPair<TObjectPtr<UWeatherAsset>, TSharedPtr<FStreamableHandle>>& Kv : LoadHandles)
    {
        FAssetManagerUtil::CancelHandle(Kv.Value);
    }
    LoadHandles.Empty();

    for (TObjectPtr<AWeatherEffectActor> Actor : EffectActors)
    {
        if (IsValid(Actor))
        {
            Actor->Destroy();
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

