// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "UObject/Interface.h"

// Generated Headers
#include "SubsystemInterface.generated.h"


/**
 *
 */
UINTERFACE(MinimalAPI, Blueprintable, DisplayName = "Subsystem Interface")
class USubsystemInterface : public UInterface
{
	GENERATED_BODY()
};


/**
 *
 */
class RENCORE_API ISubsystemInterface
{

	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Meta = (ForceAsFunction))
	void PostInitialize();

protected:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Meta = (ForceAsFunction, BlueprintProtected))
	void OnInitialized();


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Meta = (ForceAsFunction, BlueprintProtected))
	void OnDeinitialized();

};

