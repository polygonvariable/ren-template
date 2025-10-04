// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

// Project Headers
#include "RCoreCounter/Public/CounterManagerInterface.h"

// Generated Headers
#include "CounterSubsystem.generated.h"

// Forward Declarations
class ICounterProviderInterface;

struct FCounterRecord;



/**
 *
 */
UCLASS()
class RENCOUNTER_API UCounterSubsystem : public UGameInstanceSubsystem/*, public ICounterManagerInterface*/
{

	GENERATED_BODY()

public:

	// ~ ICounterManagerInterface
	virtual int AddCounter(FName Name, bool bAutoIncrement = true, int Max = INT_MAX) /*override*/;
	virtual bool RemoveCounter(FName Name) /*override*/;

	virtual int IncrementCounter(FName Name, int Max = INT_MAX) /*override*/;
	virtual int DecrementCounter(FName Name) /*override*/;

	virtual bool ContainsCounter(FName Name) const /*override*/;
	virtual int GetCounter(FName Name) const /*override*/;


	virtual int AddCounter(FName ContainerId, FName Name, bool bAutoIncrement = true, int Max = INT_MAX) /*override*/;
	virtual bool RemoveCounter(FName ContainerId, FName Name) /*override*/;
	virtual bool ResetCounter(FName ContainerId) /*override*/;

	virtual int IncrementCounter(FName ContainerId, FName Name, int Max = INT_MAX) /*override*/;
	virtual int DecrementCounter(FName ContainerId, FName Name) /*override*/;

	virtual bool ContainsCounter(FName ContainerId, FName Name) const /*override*/;
	virtual const FCounterRecord* GetCounterRecord(FName ContainerId, FName Name) const /*override*/;
	virtual const FCounterRecord* FindOrAddCounter(FName ContainerId, FName Name);
	// ~ End of ICounterManagerInterface

protected:

	TMap<FName, int> LocalCounters;
	TWeakInterfacePtr<ICounterProviderInterface> CounterProvider;


	TMap<FName, FCounterRecord>* FindOrAddCounters(FName ContainerId);
	const TMap<FName, FCounterRecord>* GetCounters(FName ContainerId) const;

	FCounterRecord* GetCounterRecordX(FName ContainerId, FName Name);


	virtual void HandleStorageLoaded();

protected:

 	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const /*override*/;
	virtual void Initialize(FSubsystemCollectionBase& Collection) /*override*/;
	virtual void Deinitialize() /*override*/;
	// ~ End of UGameInstanceSubsystem

};

