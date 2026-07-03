// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Module Macros
#define REN_API RCORELIBRARY_API

// Forward declarations
struct FInstancedPropertyBag;


/*
 *
 */
namespace FObjectUtil
{

	REN_API void CopyPropertiesToInstance(const FInstancedPropertyBag& Properties, UObject* Object);
	REN_API void CopyPropertiesToBag(UClass* Class, FInstancedPropertyBag& OutProperties);
	REN_API void CleanupBagProperties(UClass* Class, FInstancedPropertyBag& OutProperties);

};


// Module Macros
#undef REN_API

