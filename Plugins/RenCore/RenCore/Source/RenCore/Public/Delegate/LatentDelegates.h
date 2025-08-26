// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers

// Generated Headers

// Forward Declarations
class USaveGame;



class RENCORE_API FLatentDelegates
{

public:

	DECLARE_MULTICAST_DELEGATE(FLatentTaskDelegate);

	static FLatentTaskDelegate OnStorageLoaded;
	static FLatentTaskDelegate OnPreGameInitialized;
	static FLatentTaskDelegate OnPostGameInitialized;

};


