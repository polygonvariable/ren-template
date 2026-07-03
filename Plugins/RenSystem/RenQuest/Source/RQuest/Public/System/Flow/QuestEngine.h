// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "EventflowEngine.h"

// Generated Headers
#include "QuestEngine.generated.h"


/**
 *
 */
UCLASS(MinimalAPI)
class UQuestEngine : public UEventflowEngine
{

	GENERATED_BODY()

protected:

	// ~ UEventflowEngine
	virtual void GetAssetBundle(TArray<FName>& OutBundle) const override;
	// ~ End of UEventflowEngine

};

