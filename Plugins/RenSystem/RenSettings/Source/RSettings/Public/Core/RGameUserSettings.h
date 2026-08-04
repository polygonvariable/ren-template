// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameFramework/GameUserSettings.h"

// Generated Headers
#include "RGameUserSettings.generated.h"

// Forward Decleration
class IConsoleVariable;


/**
 *
 */
UCLASS(MinimalAPI)
class URGameUserSettings : public UGameUserSettings
{

    GENERATED_BODY()

public:

    UPROPERTY(Config)
    int RenderResolution = 90;

    UPROPERTY(Config)
    int AntiAliasingMethod = 5;

    UPROPERTY(Config)
    int TonemapperSharpen = 0;


    void RegisterCVar();
    void UnregisterCVar();

    // ~ UGameUserSettings
    virtual void LoadSettings(bool bForceReload) override;
    virtual void ApplySettings(bool bCheckForCommandLineOverrides) override;
    virtual void SaveSettings() override;
    // ~ End of UGameUserSettings

protected:

    IConsoleVariable* RenderCVar = nullptr;
    IConsoleVariable* AACVar = nullptr;
    IConsoleVariable* SharpenCVar = nullptr;
    IConsoleVariable* MaxFPSCVar = nullptr;


    void BindCVarDelegate(IConsoleVariable* Variable);
    void ClearCVarDelegate(IConsoleVariable* Variable);

    void UpdateCVarValues();
    void UpdateSettingValues();

    void UpdateCVarValue(IConsoleVariable* Variable, int Value, int Min, int Max);
    void UpdateSettingValue(IConsoleVariable* Variable, int& Value, int Min, int Max);
    void UpdateSettingValue(IConsoleVariable* Variable, float& Value, int Min, int Max);

    // ~ Binding
    void HandleOnCVarChanged(IConsoleVariable* Variable);
    // ~ End of Binding

private:

    bool _bCVarLoaded = false;

};


// Module Macros
#undef REN_API

