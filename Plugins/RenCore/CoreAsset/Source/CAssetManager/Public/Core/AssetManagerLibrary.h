// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Forward Declaration
struct FStreamableHandle;


/**
 *
 */
namespace FAssetManagerLibrary
{

	CASSETMANAGER_API void CancelHandle(TSharedPtr<FStreamableHandle>& SpawnHandle);
	CASSETMANAGER_API void ReleaseHandle(TSharedPtr<FStreamableHandle>& SpawnHandle);

};

