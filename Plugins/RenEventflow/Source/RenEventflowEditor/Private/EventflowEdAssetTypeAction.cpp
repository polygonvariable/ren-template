// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EventflowEdAssetTypeAction.h"

// Engine Headers

// Project Headers
#include "RenEventflow/Public/EventflowAsset.h"

#include "RenEventflowEditor/Public/EventflowEdApp.h"


EventflowEdAssetTypeAction::EventflowEdAssetTypeAction(EAssetTypeCategories::Type InAssetCategory)
{
    AssetCategory = InAssetCategory;
}

FText EventflowEdAssetTypeAction::GetName() const
{
    return FText::FromString(TEXT("Eventflow Graph"));
}

FColor EventflowEdAssetTypeAction::GetTypeColor() const
{
    return FColor::Orange;
}

UClass* EventflowEdAssetTypeAction::GetSupportedClass() const
{
    return UEventflowAsset::StaticClass();
}

uint32 EventflowEdAssetTypeAction::GetCategories()
{
    return AssetCategory;
}

void EventflowEdAssetTypeAction::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
    EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
    for (auto Obj : InObjects)
	{
        UEventflowAsset* Graph = Cast<UEventflowAsset>(Obj);
        if(Graph != nullptr)
		{
            TSharedRef<EventflowEdApp> App(new EventflowEdApp());
            App->InitEditor(Mode, EditWithinLevelEditor, Graph);
		}
	}
}
