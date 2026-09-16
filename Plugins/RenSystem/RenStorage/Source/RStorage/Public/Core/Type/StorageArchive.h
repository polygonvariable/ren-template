// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"


/**
 * 
 */
struct FStorageArchive : public FObjectAndNameAsStringProxyArchive
{
	FStorageArchive(FArchive& InInnerArchive, bool bIsLoading = false) : FObjectAndNameAsStringProxyArchive(InInnerArchive, false)
	{
		ArIsSaveGame = true;
		ArNoDelta = true;
	}
};

