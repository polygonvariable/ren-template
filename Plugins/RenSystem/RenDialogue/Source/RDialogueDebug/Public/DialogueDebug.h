// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "SlateIMWidgetBase.h"

// Forward Decalaration
class UDialogueSubsystem;


/*
 * 
 */
class FDialogueDebugWidget : public FSlateIMWidgetBase
{

public:

    FDialogueDebugWidget() :FSlateIMWidgetBase(TEXT("DialogueDebugWidget")) {};

    // ~ FSlateIMWidgetBase
    virtual void EnableWidget() override;
    virtual void DisableWidget() override;
    // ~ End of FSlateIMWidgetBase

protected:

    TWeakObjectPtr<UDialogueSubsystem> DialogueSubsystem;


    UDialogueSubsystem* GetSubsystem();

    // ~ FSlateIMWidgetBase
    virtual void DrawWidget(float DeltaTime) override;
    // ~ End of FSlateIMWidgetBase

};

