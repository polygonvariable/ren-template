// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"

// Generated Headers
#include "GameplayModeProvider.generated.h"

// Forward Declaration
class UPrimaryDataAsset;

// Delegate Declarations
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnGameplayModeTagsChanged, FGameplayTag /* Tag */, bool /* bAdded */);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameplayModeTagChanged, bool /* bAdded */);


UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UGameplayModeProvider : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class IGameplayModeProvider
{

	GENERATED_BODY()

public:

	FOnGameplayModeTagsChanged& GetOnGameplayModeTagsChanged()
	{
		return OnGameplayModeTagsChanged;
	};

	virtual bool HasTagExact(FGameplayTag Tag) const
	{
		return GetGameplayModeTags().HasTagExact(Tag);
	}

	virtual void RegisterTagNotify(FGameplayTag Tag, FOnGameplayModeTagChanged::FDelegate&& Callback) = 0;
	virtual void UnregisterTagNotify(FGameplayTag Tag, UObject* Target) = 0;

	virtual const FGameplayTagContainer& GetGameplayModeTags() const = 0;
	virtual void SetGameplayModeByTag(FGameplayTagContainer Tags) = 0;
	virtual void SetGameplayMode(UPrimaryDataAsset* Asset) = 0;

	virtual void AddGameplayMode(FGameplayTagContainer Tags) = 0;
	virtual void RempoveGameplayMode(FGameplayTagContainer Tags) = 0;

protected:

	FOnGameplayModeTagsChanged OnGameplayModeTagsChanged;

};

