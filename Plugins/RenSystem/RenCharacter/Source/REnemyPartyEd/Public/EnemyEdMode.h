// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "EdMode.h"

/**
 *
 */
class FEnemyEdMode : public FEdMode
{

public:

    static const FEditorModeID EM_EnemyManager;

    virtual void Enter() override;
    virtual void Exit() override;
    virtual void Tick(FEditorViewportClient* ViewportClient, float DeltaTime) override;

    bool bDrawDebug = true;

protected:

    void DrawDebug();

};

