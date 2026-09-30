// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Forward declarations
struct FInstancedPropertyBag;


/*
 *
 */
namespace FPropertyBagLibrary
{

	CLIBRARY_API void CopyPropertiesToInstance(const FInstancedPropertyBag& Properties, UObject* Object);
	CLIBRARY_API void CopyPropertiesToBag(UClass* Class, FInstancedPropertyBag& OutProperties);
	CLIBRARY_API void CleanupBagProperties(UClass* Class, FInstancedPropertyBag& OutProperties);

};

