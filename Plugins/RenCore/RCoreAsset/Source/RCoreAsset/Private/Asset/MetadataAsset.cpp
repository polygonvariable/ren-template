// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Asset/MetadataAsset.h"


FPrimaryAssetId UMetadataAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(TEXT("Asset.Metadata"), GetFName());
}

