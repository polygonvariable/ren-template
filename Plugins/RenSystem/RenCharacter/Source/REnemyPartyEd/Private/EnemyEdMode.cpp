// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EnemyEdMode.h"

// Engine Headers
#include "DrawDebugHelpers.h"
#include "EditorModeManager.h"
#include "Engine/Selection.h"
#include "GameFramework/Actor.h"
#include "Toolkits/ToolkitManager.h"

// Project Headers
#include "Component/EnemyManagerComponent.h"
#include "EnemyEdModeToolkit.h"


const FEditorModeID FEnemyEdMode::EM_EnemyManager = TEXT("EM_EnemyManager");

void FEnemyEdMode::Enter()
{
    FEdMode::Enter();

    if (!Toolkit.IsValid())
    {
		Toolkit = MakeShareable(new FEnemyEdModeToolkit);
		Toolkit->Init(Owner->GetToolkitHost());
    }
}

void FEnemyEdMode::Exit()
{
    FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
	Toolkit.Reset();

    FEdMode::Exit();
}

void FEnemyEdMode::Tick(FEditorViewportClient* ViewportClient, float DeltaTime)
{
    FEdMode::Tick(ViewportClient, DeltaTime);
    DrawDebug();
}

void FEnemyEdMode::DrawDebug()
{
    if (!bDrawDebug)
    {
        return;
    }

    USelection* SelectedActors = GEditor->GetSelectedActors();
    if (SelectedActors->Num() <= 0)
    {
        return;
    }

    for (int32 i = 0; i < SelectedActors->Num(); i++)
    {
        AActor* Actor = Cast<AActor>(SelectedActors->GetSelectedObject(i));
        if (!IsValid(Actor))
        {
            continue;
        }

        UEnemyManagerComponent* Manager = Actor->FindComponentByClass<UEnemyManagerComponent>();
        if (!IsValid(Manager))
        {
			continue;
        }

        const TArray<FEnemySpawnData>& SpawnData = Manager->SpawnData;
        for (const FEnemySpawnData& Data : SpawnData)
        {
            FTransform Transform = Data.EnemyData.SpawnTransform * Actor->GetActorTransform();
            FColor Color = FColor::Green;

            if (!Data.CharacterData.IsValid())
            {
                Color = FColor::Red;
            }

            DrawDebugSphere(GetWorld(), Transform.GetLocation(), 50.0f, 16, Color, false, 0.0f);
        }
    }
}


