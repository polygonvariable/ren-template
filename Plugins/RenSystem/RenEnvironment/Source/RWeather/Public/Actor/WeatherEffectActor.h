// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameFramework/Actor.h"

// Generated Headers
#include "WeatherEffectActor.generated.h"

// Forward Declarations
class USceneComponent;
class UNiagaraComponent;
class UAudioComponent;
class UNiagaraSystem;
class UArrowComponent;
class UWeatherAsset;


/**
 *
 */
UCLASS(NotBlueprintable, NotPlaceable)
class AWeatherEffectActor : public AActor
{

    GENERATED_BODY()

public:

	AWeatherEffectActor();

    FSoftObjectPath EffectPath;

    virtual bool InitializeEffect();
    virtual void DeinitializeEffect();

    virtual void ActivateEffect();
    virtual void DeactivateEffect();

#if UE_BUILD_DEVELOPMENT
    virtual UActorComponent* GetEditorEffectComponent() const;
#endif

protected:

    UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
    TObjectPtr<USceneComponent> SceneComponent;

#if WITH_EDITORONLY_DATA
    UPROPERTY(VisibleDefaultsOnly)
    TObjectPtr<UArrowComponent> ArrowComponent;
#endif

    FTimerHandle FollowTimer;


    void CreateFollow();
    void RemoveFollow();

    // ~ Binding
    void HandleOnTimerTick();
    // ~ End of Binding

    // ~ AActor
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    // ~ End of AActor

};


/**
 *
 */
UCLASS(NotBlueprintable, NotPlaceable)
class AWeatherEffectParticle : public AWeatherEffectActor
{

    GENERATED_BODY()

public:

    AWeatherEffectParticle();

    // ~ AWeatherEffectActor
    virtual bool InitializeEffect() override;
    virtual void ActivateEffect() override;
    virtual void DeactivateEffect() override;
    // ~ End of AWeatherEffectActor

#if UE_BUILD_DEVELOPMENT
    virtual UActorComponent* GetEditorEffectComponent() const;
#endif

protected:

    UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
    TObjectPtr<UNiagaraComponent> NiagaraComponent;

};


/**
 *
 */
UCLASS(NotBlueprintable, NotPlaceable)
class AWeatherEffectAudio : public AWeatherEffectActor
{

    GENERATED_BODY()

public:

    AWeatherEffectAudio();

    // ~ AWeatherEffectActor
    virtual bool InitializeEffect() override;
    virtual void ActivateEffect() override;
    virtual void DeactivateEffect() override;
    // ~ End of AWeatherEffectActor

#if UE_BUILD_DEVELOPMENT
    virtual UActorComponent* GetEditorEffectComponent() const;
#endif

protected:

    UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
    TObjectPtr<UAudioComponent> AudioComponent;

};

