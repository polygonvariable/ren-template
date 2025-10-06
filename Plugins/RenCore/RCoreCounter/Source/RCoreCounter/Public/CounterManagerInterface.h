// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "UObject/Interface.h"

// Project Headers

// Generated Headers
#include "CounterManagerInterface.generated.h"

// Forward Declarations
struct FCounterRecord;



UINTERFACE(MinimalAPI)
class UCounterManagerInterface : public UInterface
{

	GENERATED_BODY()

};

/**
 *
 */
class RCORECOUNTER_API ICounterManagerInterface
{

	GENERATED_BODY()

public:

	virtual int AddCounter(FName Name, bool bAutoIncrement = true, int Max = INT_MAX) = 0;
	virtual bool RemoveCounter(FName Name) = 0;

	virtual int IncrementCounter(FName Name, int Max = INT_MAX) = 0;
	virtual int DecrementCounter(FName Name) = 0;

	virtual bool ContainsCounter(FName Name) const = 0;
	virtual int GetCounter(FName Name) const = 0;


	virtual int AddCounter(FName ContainerId, FName Name, bool bAutoIncrement = true, int Max = INT_MAX) = 0;
	virtual bool RemoveCounter(FName ContainerId, FName Name) = 0;
	virtual bool ResetCounter(FName ContainerId) = 0;

	virtual int IncrementCounter(FName ContainerId, FName Name, int Max = INT_MAX) = 0;
	virtual int DecrementCounter(FName ContainerId, FName Name) = 0;

	virtual bool ContainsCounter(FName ContainerId, FName Name) const = 0;
	virtual const FCounterRecord* GetCounterRecord(FName ContainerId, FName Name) const = 0;
	virtual const FCounterRecord* FindOrAddCounter(FName ContainerId, FName Name) = 0;

};

