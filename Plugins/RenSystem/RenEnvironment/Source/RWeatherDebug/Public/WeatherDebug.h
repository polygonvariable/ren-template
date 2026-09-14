// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "SlateIMWidgetBase.h"

// Forward Declaration
class UWeatherController;
class AWeatherEffectManagerActor;


/*
 * 
 */
class FWeatherDebugWidget : public FSlateIMWidgetBase
{

public:

    FWeatherDebugWidget() :FSlateIMWidgetBase(TEXT("WeatherDebugWidget")) {};


    // ~ FSlateIMWidgetBase
    virtual void EnableWidget() override;
    virtual void DisableWidget() override;
    // ~ End of FSlateIMWidgetBase

protected:

    TWeakObjectPtr<UWeatherController> WeatherController;
    TWeakObjectPtr<AWeatherEffectManagerActor> WeatherManager;

    UWorld* GetWorld();
    UWeatherController* GetController();
    AWeatherEffectManagerActor* GetWeatherManager();


    void Draw_WeatherManager(AWeatherEffectManagerActor* Manager);
    void Draw_WeatherManagerEffects(AWeatherEffectManagerActor* Manager);
    void Draw_WeatherManagerEffectHandles(AWeatherEffectManagerActor* Manager);

    void Draw_WeatherControllerActive(UWeatherController* Controller);
    void Draw_WeatherControllerList(UWeatherController* Controller);

    // ~ FSlateIMWidgetBase
    virtual void DrawWidget(float DeltaTime) override;
    // ~ End of FSlateIMWidgetBase

};

