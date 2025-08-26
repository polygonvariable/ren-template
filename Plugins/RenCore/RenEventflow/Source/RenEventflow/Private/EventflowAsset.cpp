// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EventflowAsset.h"

// Engine Headers

// Project Headers





void UEventflowNodeData::PreSave(FObjectPreSaveContext ObjectSaveContext)
{
	Super::PreSave(ObjectSaveContext);
	UE_LOG(LogTemp, Warning, TEXT("UEventflowNodeData::PreSave"));
}
void UEventflowNodeData::PreSaveRoot(FObjectPreSaveRootContext ObjectSaveContext)
{
	Super::PreSaveRoot(ObjectSaveContext);
	//GEngine->ForceGarbageCollection(true);
	//ObjectSaveContext.SetCleanupRequired(true);
	UE_LOG(LogTemp, Warning, TEXT("UEventflowNodeData::PreSaveRoot"));
}



void UEventflowPin::PreSave(FObjectPreSaveContext ObjectSaveContext)
{
	Super::PreSave(ObjectSaveContext);
	UE_LOG(LogTemp, Warning, TEXT("UEventflowPin::PreSave"));
}
void UEventflowPin::PreSaveRoot(FObjectPreSaveRootContext ObjectSaveContext)
{
	Super::PreSaveRoot(ObjectSaveContext);
	//GEngine->ForceGarbageCollection(true);
	//ObjectSaveContext.SetCleanupRequired(true);
	UE_LOG(LogTemp, Warning, TEXT("UEventflowPin::PreSaveRoot"));
}



void UEventflowNode::PreSave(FObjectPreSaveContext ObjectSaveContext)
{
	Super::PreSave(ObjectSaveContext);
	UE_LOG(LogTemp, Warning, TEXT("UEventflowNode::PreSave"));
}
void UEventflowNode::PreSaveRoot(FObjectPreSaveRootContext ObjectSaveContext)
{
	Super::PreSaveRoot(ObjectSaveContext);
	//GEngine->ForceGarbageCollection(true);
	//ObjectSaveContext.SetCleanupRequired(true);
	UE_LOG(LogTemp, Warning, TEXT("UEventflowNode::PreSaveRoot"));
}



void UEventflowData::PreSave(FObjectPreSaveContext ObjectSaveContext)
{
	Super::PreSave(ObjectSaveContext);
	UE_LOG(LogTemp, Warning, TEXT("UEventflowData::PreSave"));
}

void UEventflowData::PreSaveRoot(FObjectPreSaveRootContext ObjectSaveContext)
{
	Super::PreSaveRoot(ObjectSaveContext);
	//GEngine->ForceGarbageCollection(true);
	//ObjectSaveContext.SetCleanupRequired(true);
	UE_LOG(LogTemp, Warning, TEXT("UEventflowData::PreSaveRoot"));
}


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
