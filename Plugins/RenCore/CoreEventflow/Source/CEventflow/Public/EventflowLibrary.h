// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Forward Declarations
class IEventflowEngineProvider;


/**
 *
 */
class CEVENTFLOW_API FEventflowLibrary
{

public:

	static IEventflowEngineProvider* GetEngineProvider(UWorld* Context, const FPrimaryAssetId& AssetId);
	static IEventflowEngineProvider* GetEngineProvider(UWorld* Context, const FPrimaryAssetType& AssetType);

};

