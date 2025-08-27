// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EventflowAsset.h"

// Engine Headers

// Project Headers



void UEventflowAsset::PreSave(FObjectPreSaveContext ObjectSaveContext)
{
	Super::PreSave(ObjectSaveContext);
	UE_LOG(LogTemp, Warning, TEXT("UEventflowAsset::PreSave"));
}

void UEventflowAsset::PreSaveRoot(FObjectPreSaveRootContext ObjectSaveContext)
{
	Super::PreSaveRoot(ObjectSaveContext);
	OnEventflowAssetSaved.Broadcast();

	GEngine->ForceGarbageCollection(true);
	//ObjectSaveContext.SetCleanupRequired(true);
	UE_LOG(LogTemp, Warning, TEXT("UEventflowAsset::PreSaveRoot"));
}
