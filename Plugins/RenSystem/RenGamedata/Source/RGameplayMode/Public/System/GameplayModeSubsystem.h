// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/WorldSubsystem.h"

// Project Headers
#include "Core/GameplayModeProvider.h"
#include "Priority/PriorityListInterface.h"
#include "Data/Asset/MetadataAsset.h"
#include "WorldConfigAsset.h"

// Generated Headers
#include "GameplayModeSubsystem.generated.h"

// Forward Declaration
class UGameplayModeAsset;


/**
 * 
 */
UCLASS(MinimalAPI)
class UGameplayModeSubsystem : public UWorldSubsystem, public IGameplayModeProvider, public IPriorityListInterface
{

	GENERATED_BODY()

public:



	virtual const FGameplayTagContainer& GetGameplayModeTags() const;

	virtual void RegisterTagNotify(FGameplayTag Tag, FOnGameplayModeTagChanged::FDelegate&& Callback) override;
	virtual void UnregisterTagNotify(FGameplayTag Tag, UObject* Target) override;


	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set Gameplay Mode", Categories = "Gameplay"))
	void BP_SetGameplayMode(UGameplayModeAsset* Asset);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set Gameplay Mode By Tag", Categories = "Gameplay"))
	void BP_SetGameplayModeByTag(FGameplayTagContainer Tags);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Add Gameplay Mode", Categories = "Gameplay"))
	void BP_AddGameplayMode(FGameplayTagContainer Tags);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Remove Gameplay Mode", Categories = "Gameplay"))
	void BP_RempoveGameplayMode(FGameplayTagContainer Tags);

	// ~ IGameplayModeProvider
	virtual void SetGameplayMode(UPrimaryDataAsset* Asset) override;
	virtual void SetGameplayModeByTag(FGameplayTagContainer Tags) override;
	virtual void AddGameplayMode(FGameplayTagContainer Tags) override;
	virtual void RempoveGameplayMode(FGameplayTagContainer Tags) override;
	// ~ End of IGameplayModeProvider
	
	// ~ UWorldSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Deinitialize() override;
	// ~ End of UWorldSubsystem

protected:

	bool bCanBroadcast = false;
	TMap<FGameplayTag, TPair<FOnGameplayModeTagChanged, int>> Handles;
	FGameplayTagContainer GameplayModeTag;

	TMap<FGameplayTag, FGameplayTagContainer> GameplayModeCategory;


	// ~ IPriorityListInterface
	virtual TMap<int, TWeakObjectPtr<UObject>>& GetPriorityItems() override final;
	virtual void OnPriorityItemChanged(UObject* PreviousItem, UObject* NewItem) override;
	virtual void OnPriorityItemRemoved(UObject* Item, bool bReplaced) override;
	// ~ End of IPriorityListInterface

	// ~ UWorldSubsystem
	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;
	// ~ End of UWorldSubsystem

private:

	UPROPERTY()
	TMap<int, TWeakObjectPtr<UObject>> GameplayModes;

public:

	static RGAMEPLAYMODE_API UGameplayModeSubsystem* Get(UWorld* World);

};


/**
 *
 */
UCLASS(MinimalAPI)
class UGameplayModeWorldConfig : public UWorldConfigAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Mode")
	bool bEnabled = true;

	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (Categories = "Gameplay"))
	FGameplayTagContainer DefaultMode;

#if WITH_EDITOR
	// ~ UPrimaryDataAsset
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
	// ~ End of UPrimaryDataAsset
#endif

public:

	static const UGameplayModeWorldConfig* Get(UWorld* World);

};

