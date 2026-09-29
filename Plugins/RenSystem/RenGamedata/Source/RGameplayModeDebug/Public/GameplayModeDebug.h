// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "SlateIMWidgetBase.h"

// Forward Decalaration
class UGameplayModeSubsystem;


/*
 * 
 */
class FGameplayModeDebugWidget : public FSlateIMWidgetBase
{

public:

    FGameplayModeDebugWidget() : FSlateIMWidgetBase(TEXT("GameplayModeDebugWidget")) {};

    // ~ FSlateIMWidgetBase
    virtual void EnableWidget() override;
    virtual void DisableWidget() override;
    // ~ End of FSlateIMWidgetBase

protected:

    TWeakObjectPtr<UGameplayModeSubsystem> GameplayModeSubsystem;


    UGameplayModeSubsystem* GetSubsystem();

    // ~ FSlateIMWidgetBase
    virtual void DrawWidget(float DeltaTime) override;
    // ~ End of FSlateIMWidgetBase

};

