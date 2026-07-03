// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EventflowEdAction.h"

// Project Headers
#include "App/EventflowEdApp.h"
#include "Asset/EventflowAsset.h"


FEventflowEdAction::FEventflowEdAction(EAssetTypeCategories::Type InAssetCategory)
{
    AssetCategory = InAssetCategory;
}

FText FEventflowEdAction::GetName() const
{
    return FText::FromString(TEXT("Eventflow Graph"));
}

FColor FEventflowEdAction::GetTypeColor() const
{
    return FColor::Orange;
}

UClass* FEventflowEdAction::GetSupportedClass() const
{
    return UEventflowAsset::StaticClass();
}

uint32 FEventflowEdAction::GetCategories()
{
    return AssetCategory;
}

void FEventflowEdAction::OpenAssetEditor(const TArray<UObject*>& Objects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
    EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
    for (UObject* Object : Objects)
	{
        UEventflowAsset* Graph = Cast<UEventflowAsset>(Object);
        if(Graph != nullptr)
		{
            TSharedRef<FEventflowEdApp> App(new FEventflowEdApp());
            App->InitEditor(Mode, EditWithinLevelEditor, Graph);
		}
	}
}

