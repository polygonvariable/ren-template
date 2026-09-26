// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/WorldSubsystem.h"

// Project Headers
#include "Core/GameplayModeProvider.h"
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
class UGameplayModeSubsystem : public UWorldSubsystem, public IGameplayModeProvider
{

	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set Gameplay Mode By Tag", Categories = "Gameplay"))
	void BP_SetGameplayModeByTag(FGameplayTagContainer Tags);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Add Gameplay Mode", Categories = "Gameplay"))
	void BP_AddGameplayMode(FGameplayTagContainer Tags);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Remove Gameplay Mode", Categories = "Gameplay"))
	void BP_RempoveGameplayMode(FGameplayTagContainer Tags);

	// ~ IGameplayModeProvider
	virtual void RegisterTagNotify(FGameplayTag Tag, FOnGameplayModeTagChanged::FDelegate&& Callback) override;
	virtual void UnregisterTagNotify(FGameplayTag Tag, UObject* Target) override;

	virtual const FGameplayTagContainer& GetGameplayModeTags() const;
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


	// ~ UWorldSubsystem
	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;
	// ~ End of UWorldSubsystem

public:

	static UGameplayModeSubsystem* Get(UWorld* World);

};

