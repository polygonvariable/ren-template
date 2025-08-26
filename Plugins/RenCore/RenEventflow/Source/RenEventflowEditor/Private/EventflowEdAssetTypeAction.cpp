// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EventflowEdAssetTypeAction.h"

// Engine Headers

// Project Headers
#include "RenEventflow/Public/EventflowAsset.h"

#include "RenEventflowEditor/Public/App/EventflowEdApp.h"



FEventflowEdAssetTypeAction::FEventflowEdAssetTypeAction(EAssetTypeCategories::Type InAssetCategory)
{
    AssetCategory = InAssetCategory;
}

FText FEventflowEdAssetTypeAction::GetName() const
{
    return FText::FromString(TEXT("Eventflow Graph"));
}

FColor FEventflowEdAssetTypeAction::GetTypeColor() const
{
    return FColor::Orange;
}

UClass* FEventflowEdAssetTypeAction::GetSupportedClass() const
{
    return UEventflowAsset::StaticClass();
}

uint32 FEventflowEdAssetTypeAction::GetCategories()
{
    return AssetCategory;
}

void FEventflowEdAssetTypeAction::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
    EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
    for (auto Obj : InObjects)
	{
        UEventflowAsset* Graph = Cast<UEventflowAsset>(Obj);
        if(Graph != nullptr)
		{
            TSharedRef<FEventflowEdApp> App(new FEventflowEdApp());
            App->InitEditor(Mode, EditWithinLevelEditor, Graph);
		}
	}
}

