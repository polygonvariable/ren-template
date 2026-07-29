// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EventflowAsset.h"

#if WITH_EDITOR
// Engine Headers
#include "Misc/DataValidation.h"

// Project Headers
#include "Task/EventflowPrimaryTask.h"
#endif


FPrimaryAssetType UEventflowAsset::GetPrimaryAssetType()
{
	return TEXT("Asset.Eventflow");
}

#if WITH_EDITOR

void UEventflowAsset::PreSaveRoot(FObjectPreSaveRootContext ObjectSaveContext)
{
	Super::PreSaveRoot(ObjectSaveContext);

	GEngine->ForceGarbageCollection(true);
	UE_LOG(LogTemp, Warning, TEXT("UEventflowAsset::PreSaveRoot"));
}

void UEventflowAsset::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);
}

EDataValidationResult UEventflowAsset::IsDataValid(FDataValidationContext& Context) const
{
	return EDataValidationResult::Valid;
}

void UEventflowAsset::UpdateAssetBundleData()
{
	Super::UpdateAssetBundleData();

	for (const TPair<FGuid, FEventflowNode>& Kv : NodeCollection)
	{
		UEventflowPrimaryTask* Task = Kv.Value.Task;
		if (IsValid(Task))
		{
			Task->AppendAssetBundleData(AssetBundleData);
		}
	}

	AssetBundleData;
}

#endif

FPrimaryAssetId UEventflowAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(UEventflowAsset::GetPrimaryAssetType(), GetFName());
}

