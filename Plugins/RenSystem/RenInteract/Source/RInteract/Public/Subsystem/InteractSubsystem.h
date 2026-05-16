// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/WorldSubsystem.h"

// Project Headers
#include "Definition/InteractItem.h"

// Generated Headers
#include "InteractSubsystem.generated.h"

// Module Macros
#define REN_API RINTERACT_API


/**
 *
 */
UCLASS()
class UInteractSubsystem : public UWorldSubsystem
{

	GENERATED_BODY()

public:
	
	DECLARE_DELEGATE_TwoParams(FOnInteractAdded, const FGuid&, const FInteractItem&);
	DECLARE_DELEGATE_OneParam(FOnInteractRemoved, const FGuid&);

	FOnInteractAdded OnInteractAdded;
	FOnInteractRemoved OnInteractRemoved;


	void RegisterItem(const FGuid& InteractId, AActor* Actor, const FInteractItem& InteractItem);
	void UnregisterItem(const FGuid& InteractId);

	void InteractItemById(const FGuid& InteractId);

protected:

	TMap<FGuid, TPair<TWeakObjectPtr<AActor>, FInteractItem>> RegisteredItems;
	
	// ~ UWorldSubsystem
	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UWorldSubsystem

public:

	static REN_API UInteractSubsystem* Get(const UWorld* World);

};


// Module Macros
#undef REN_API

