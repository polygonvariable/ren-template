// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Core/AssetManagerUtil.h"

// Engine Headers
#include "Engine/StreamableManager.h"


void FAssetManagerUtil::CancelHandle(TSharedPtr<FStreamableHandle>& SpawnHandle)
{
	if (SpawnHandle.IsValid())
	{
		SpawnHandle->CancelHandle();
		SpawnHandle->ReleaseHandle();
		SpawnHandle.Reset();
	}
}

void FAssetManagerUtil::ReleaseHandle(TSharedPtr<FStreamableHandle>& SpawnHandle)
{
	if (SpawnHandle.IsValid())
	{
		SpawnHandle->ReleaseHandle();
		SpawnHandle.Reset();
	}
}

