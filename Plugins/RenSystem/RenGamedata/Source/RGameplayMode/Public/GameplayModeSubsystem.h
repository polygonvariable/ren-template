// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/WorldSubsystem.h"

// Project Headers
#include "GameplayModeProvider.h"

// Generated Headers
#include "GameplayModeSubsystem.generated.h"

// Forward Declaration
class UDataTable;
class UGameplayModeAsset;
struct FStreamableHandle;


/**
 * 
 */
UCLASS(MinimalAPI)
class UGameplayModeSubsystem : public UWorldSubsystem, public IGameplayModeProvider
{

	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Push Gameplay Mode"))
	void BP_PushGameplayMode(FName Mode);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Pop Gameplay Mode"))
	void BP_PopGameplayMode(FName Mode);

	// ~ IGameplayModeProvider
	virtual void PushGameplayMode(FName Mode) override;
	virtual void PopGameplayMode(FName Mode) override;
	virtual void RegisterTagNotify(FGameplayTag Tag, FOnGameplayModeTagChanged::FDelegate&& Callback) override;
	virtual void UnregisterTagNotify(FGameplayTag Tag, UObject* Target) override;
	RGAMEPLAYMODE_API virtual const FGameplayTagContainer& GetGameplayModeTags() const;
	// ~ End of IGameplayModeProvider
	
	// ~ UWorldSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Deinitialize() override;
	// ~ End of UWorldSubsystem

#if UE_BUILD_DEVELOPMENT
	RGAMEPLAYMODE_API const TArray<FName>& GetEditorGameplayModeStack() const;
#endif

protected:

	FGameplayTagContainer GameplayModeTag;

	TArray<FName> GameplayModeStack;

	bool bCanBroadcast = false;

	TMap<FGameplayTag, TPair<FOnGameplayModeTagChanged, int>> Handles;

	UPROPERTY()
	TObjectPtr<UDataTable> GameplayModeTable;

	TSharedPtr<FStreamableHandle> TableHandle;


	void BroadcastTagChange(FGameplayTag Tag, bool bAdded);

	// ~ IGameplayModeProvider
	virtual void AddGameplayMode(FGameplayTagContainer Tags) override;
	virtual void RempoveGameplayMode(FGameplayTagContainer Tags) override;
	// ~ End of IGameplayModeProvider

	// ~ Binding
	void HandleOnGameplayModeTableLoaded();
	// ~ End of Binding

	// ~ UWorldSubsystem
	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;
	// ~ End of UWorldSubsystem

public:

	static RGAMEPLAYMODE_API UGameplayModeSubsystem* Get(UWorld* World);

};

