// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/WorldSubsystem.h"

// Project Headers
#include "InteractItem.h"

// Generated Headers
#include "InteractSubsystem.generated.h"

// Forward Declaration
class UInteractComponent;


/**
 *
 */
UCLASS()
class UInteractSubsystem : public UWorldSubsystem
{

	GENERATED_BODY()

public:
	
	DECLARE_DELEGATE_TwoParams(FOnInteractAdded, const FGuid&, const FInteractItem&);
	FOnInteractAdded OnInteractAdded;

	DECLARE_DELEGATE_OneParam(FOnInteractRemoved, const FGuid&);
	FOnInteractRemoved OnInteractRemoved;


	void RegisterItem(const FGuid& InteractId, UInteractComponent* Interact, const FInteractItem& InteractItem);
	void UnregisterItem(const FGuid& InteractId);

	RINTERACT_API void InteractItemById(const FGuid& InteractId);

protected:

	bool bSetMode = false;
	TMap<FGuid, TPair<TWeakObjectPtr<UInteractComponent>, FInteractItem>> RegisteredItems;
	
	// ~ UWorldSubsystem
	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UWorldSubsystem

public:

	static RINTERACT_API UInteractSubsystem* Get(const UWorld* World);

};

