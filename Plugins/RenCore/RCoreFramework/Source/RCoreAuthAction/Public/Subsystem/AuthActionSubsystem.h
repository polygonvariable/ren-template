// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/GameInstanceSubsystem.h"

// Generated Headers
#include "AuthActionSubsystem.generated.h"

// Module Macros
#define REN_API RCOREAUTHACTION_API

// Forward Declarations
class UAuthAction;


/**
 *
 */
UCLASS(MinimalAPI, NotBlueprintType)
class UAuthActionSubsystem : public UGameInstanceSubsystem
{

	GENERATED_BODY()

public:

	REN_API UAuthAction* CreateAction(FGuid ActionId, TSubclassOf<UAuthAction> ActionClass);

	template<class T>
	T* CreateAction(FGuid ActionId)
	{
		return Cast<T>(CreateAction(ActionId, T::StaticClass()));
	}

	REN_API void StartAction(FGuid ActionId);
	REN_API void StopAction(FGuid ActionId);

protected:

	UPROPERTY()
	TMap<FGuid, TObjectPtr<UAuthAction>> ActiveActions;

	/* A free list of latent tasks */
	UPROPERTY()
	TMap<TSubclassOf<UAuthAction>, TObjectPtr<UAuthAction>> ActionList;


	UAuthAction* AcquireFromList(TSubclassOf<UAuthAction> ActionClass);
	void ReturnToList(UAuthAction* Action);

	UAuthAction* FindAction(FGuid ActionId);

	void HandleOnActionFinished(FGuid ActionId);

	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem

public:

	REN_API static UAuthActionSubsystem* Get(UWorld* World);
	REN_API static UAuthActionSubsystem* Get(UGameInstance* GameInstance);

};


// Module Macros
#undef REN_API

