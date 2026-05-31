// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EventflowAsset.h"

// Engine Headers
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif


#if WITH_EDITOR

void UEventflowAsset::PreSaveRoot(FObjectPreSaveRootContext ObjectSaveContext)
{
	Super::PreSaveRoot(ObjectSaveContext);

	GEngine->ForceGarbageCollection(true);
	UE_LOG(LogTemp, Warning, TEXT("UEventflowAsset::PreSaveRoot"));
}

EDataValidationResult UEventflowAsset::IsDataValid(FDataValidationContext& Context) const
{
	return EDataValidationResult::Valid;
}

#endif

FPrimaryAssetType UEventflowAsset::GetPrimaryAssetType()
{
	return TEXT("Asset.Eventflow");
}

FPrimaryAssetId UEventflowAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(UEventflowAsset::GetPrimaryAssetType(), GetFName());
}

