// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameUserSettings.h"

// Generated Headers
#include "HSettings.generated.h"


/**
 *
 */
UCLASS(MinimalAPI)
class URGameSettings : public UGameUserSettings
{

    GENERATED_BODY()

public:

    UPROPERTY(Config)
    int RenderResolution = 90;

    UPROPERTY(Config)
    int AntiAliasingMethod = 4;

    UPROPERTY(Config)
    int TonemapperSharpen = 0;

    // ~ UGameUserSettings
    virtual void ApplySettings(bool bCheckForCommandLineOverrides) override;
    // ~ End of UGameUserSettings

};


// Module Macros
#undef REN_API

