// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Delegate Declaration
DECLARE_MULTICAST_DELEGATE(FGameplayEventDelegate);


/**
 *
 */
class CGAMEPLAYBASE_API FGameLifecycleDelegate
{

public:


	static FGameplayEventDelegate OnPreGameInitialized;
	static FGameplayEventDelegate OnPostGameInitialized;

};

