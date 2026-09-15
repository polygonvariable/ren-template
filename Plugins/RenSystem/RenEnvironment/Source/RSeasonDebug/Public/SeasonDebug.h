// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "SlateIMWidgetBase.h"

// Forward Declaration
class USeasonController;


/*
 * 
 */
class FSeasonDebugWidget : public FSlateIMWidgetBase
{

public:

    FSeasonDebugWidget() :FSlateIMWidgetBase(TEXT("SeasonDebugWidget")) {};


    // ~ FSlateIMWidgetBase
    virtual void EnableWidget() override;
    virtual void DisableWidget() override;
    // ~ End of FSlateIMWidgetBase

protected:

    TWeakObjectPtr<USeasonController> SeasonController;

    UWorld* GetWorld();
    USeasonController* GetController();


    // ~ FSlateIMWidgetBase
    virtual void DrawWidget(float DeltaTime) override;
    // ~ End of FSlateIMWidgetBase

};

