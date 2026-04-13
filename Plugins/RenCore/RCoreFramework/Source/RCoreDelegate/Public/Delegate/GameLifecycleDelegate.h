// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Delegate/GameEventDelegate.h"

// Module Macros
#define REN_API RCOREDELEGATE_API


/**
 *
 */
class REN_API FGameLifecycleDelegate
{

public:

	static FGameEventDelegate OnPreGameInitialized;
	static FGameEventDelegate OnPostGameInitialized;

};


// Module Macros
#undef REN_API

