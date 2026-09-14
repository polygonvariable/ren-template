// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameFramework/Actor.h"

// Generated Headers
#include "WeatherEffectActor.generated.h"

// Module Macros
#define REN_API RWEATHER_API

// Forward Declarations
class USceneComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class UArrowComponent;


/**
 *
 */
UCLASS()
class AWeatherEffectActor : public AActor
{

    GENERATED_BODY()

public:

	AWeatherEffectActor();

    void SetNiagaraSystem(UNiagaraSystem* Asset);
    REN_API UNiagaraSystem* GetNiagaraSystem() const;

    void ActivateEffect();
    void DeactivateEffect();

#if WITH_EDITOR

    REN_API UNiagaraComponent* GetEditorNiagaraComponent();

    // ~ AActor
    virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
    // ~ End of AActor
#endif

protected:

    UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
    TObjectPtr<USceneComponent> SceneComponent;

    UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
    TObjectPtr<UNiagaraComponent> NiagaraComponent;

#if WITH_EDITORONLY_DATA
    UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
    TObjectPtr<UArrowComponent> ArrowComponent;
#endif

    FTimerHandle FollowTimer;


    void CreateFollowTimer();
    void RemoveFollowTimer();

    // ~ Binding
    void HandleOnTimerTick();
    // ~ End of Binding

    // ~ AActor
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    // ~ End of AActor

};


// Module Macros
#undef REN_API

