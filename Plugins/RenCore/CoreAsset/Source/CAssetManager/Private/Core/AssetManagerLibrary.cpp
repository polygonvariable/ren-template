// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Core/AssetManagerLibrary.h"

// Engine Headers
#include "Engine/StreamableManager.h"


void FAssetManagerLibrary::CancelHandle(TSharedPtr<FStreamableHandle>& SpawnHandle)
{
	if (SpawnHandle.IsValid())
	{
		SpawnHandle->CancelHandle();
		SpawnHandle->ReleaseHandle();
		SpawnHandle.Reset();
	}
}

void FAssetManagerLibrary::ReleaseHandle(TSharedPtr<FStreamableHandle>& SpawnHandle)
{
	if (SpawnHandle.IsValid())
	{
		SpawnHandle->ReleaseHandle();
		SpawnHandle.Reset();
	}
}

