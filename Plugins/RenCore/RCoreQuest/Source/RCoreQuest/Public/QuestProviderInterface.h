// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "UObject/Interface.h"

// Project Headers

// Generated Headers
#include "QuestProviderInterface.generated.h"

// Forward Declarations
struct FQuestRecord;



UINTERFACE(MinimalAPI)
class UQuestProviderInterface : public UInterface
{

	GENERATED_BODY()

};

/**
 *
 */
class RCOREQUEST_API IQuestProviderInterface
{

	GENERATED_BODY()

public:

	virtual const TMap<FName, FQuestRecord>& GetActiveQuests() const = 0;
	virtual TMap<FName, FQuestRecord>& GetMutableActiveQuests() = 0;

	virtual const TMap<FName, FDateTime>& GetCompletedQuests() const = 0;
	virtual TMap<FName, FDateTime>& GetMutableCompletedQuests() = 0;

};

