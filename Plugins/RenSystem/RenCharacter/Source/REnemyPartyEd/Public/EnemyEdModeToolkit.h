// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "EdMode.h"
#include "Toolkits/BaseToolkit.h"

// Forward Declarations
class SEnemyEdModeWidget;

/*
 * 
 */
class FEnemyEdModeToolkit : public FModeToolkit
{
public:

    FEnemyEdModeToolkit();

    virtual FName GetToolkitFName() const override;
    virtual FText GetBaseToolkitName() const override;
    virtual class FEdMode* GetEditorMode() const override;
    virtual TSharedPtr<class SWidget> GetInlineContent() const override;

private:

    TSharedPtr<SEnemyEdModeWidget> Widget;

};

