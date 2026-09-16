// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "SlateIMWidgetBase.h"

// Forward Declaration
class UClockSubsystem;


/*
 * 
 */
class FClockDebugWidget : public FSlateIMWidgetBase
{

public:

    FClockDebugWidget() :FSlateIMWidgetBase(TEXT("ClockDebugWidget")) {};


    // ~ FSlateIMWidgetBase
    virtual void DisableWidget() override;
    // ~ End of FSlateIMWidgetBase

protected:

    TWeakObjectPtr<UClockSubsystem> ClockSubsystem;


    UClockSubsystem* GetClockSubsystem();

    // ~ FSlateIMWidgetBase
    virtual void DrawWidget(float DeltaTime) override;
    // ~ End of FSlateIMWidgetBase

};

