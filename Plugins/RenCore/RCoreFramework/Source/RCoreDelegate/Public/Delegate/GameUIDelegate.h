// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Delegate/GameEventDelegate.h"

// Module Macros
#define REN_API RCOREDELEGATE_API


/**
 *
 */
class REN_API FGameUIDelegate
{

public:

	static FGameEventDelegate OnUIActionStarted;
	static FGameEventDelegate OnUIActionCompleted;

};


// Module Macros
#undef REN_API

