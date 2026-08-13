// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Module Macros
#define REN_API RCOREASSETMANAGER_API

// Forward Declaration
struct FStreamableHandle;


/**
 *
 */
namespace FAssetManagerUtil
{

	REN_API void CancelHandle(TSharedPtr<FStreamableHandle>& SpawnHandle);
	REN_API void ReleaseHandle(TSharedPtr<FStreamableHandle>& SpawnHandle);

};


// Module Macros
#undef REN_API

