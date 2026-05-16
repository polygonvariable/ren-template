// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EnemyEdModeToolkit.h"

// Engine Headers
#include "EditorModeManager.h"

// Project Headers
#include "EnemyEdMode.h"
#include "Slate/EnemyEdModeWidget.h"


FEnemyEdModeToolkit::FEnemyEdModeToolkit()
{
    SAssignNew(Widget, SEnemyEdModeWidget);
}

FName FEnemyEdModeToolkit::GetToolkitFName() const
{
    return FName("EnemyManager");
}

FText FEnemyEdModeToolkit::GetBaseToolkitName() const
{
    return NSLOCTEXT("BuilderModeToolkit", "DisplayName", "Builder");
}

FEdMode* FEnemyEdModeToolkit::GetEditorMode() const
{
    return GLevelEditorModeTools().GetActiveMode(FEnemyEdMode::EM_EnemyManager);
}

TSharedPtr<class SWidget> FEnemyEdModeToolkit::GetInlineContent() const
{
    return Widget;
}

