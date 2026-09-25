// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameplayTagContainer.h"

// Project Headers
#include "Data/Asset/MetadataAsset.h"

// Generated Headers
#include "GameplayInputManagerComponent.generated.h"

// Forward Declaration
class UInputMappingContext;
class UEnhancedInputLocalPlayerSubsystem;
class IGameplayModeProvider;
struct FStreamableHandle;


/**
 *
 */
USTRUCT()
struct FGameplayInputDefinition
{

	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, meta = (Categories = "Gameplay"))
	FGameplayTag GameplayModeTag;

	UPROPERTY(EditDefaultsOnly)
	TArray<TObjectPtr<UInputMappingContext>> InputMappings;

};


/**
 *
 */
UCLASS(MinimalAPI)
class UGameplayInputCollectionAsset : public UMetadataAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TArray<FGameplayInputDefinition> Inputs;

};


/**
 *
 */
UCLASS(MinimalAPI, meta = (BlueprintSpawnableComponent))
class UGameplayInputManagerComponent : public UActorComponent
{

	GENERATED_BODY()

public:

	UGameplayInputManagerComponent();

	// ~ UActorComponent
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// ~ End of UActorComponent

#if WITH_EDITOR
	// ~ UActorComponent
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
	// ~ End of UActorComponent
#endif

protected:

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UGameplayInputCollectionAsset> InputCollection;
	
	IGameplayModeProvider* GameplayMode;


	UEnhancedInputLocalPlayerSubsystem* GetInputSubsystem() const;
	void RegisterInputs(const FGameplayInputDefinition& Definition, bool bAdded, UEnhancedInputLocalPlayerSubsystem* InputSubsystem);

	// ~ Binding
	void HandleOnInputAssetLoaded();
	void HandleOnGameplayModeTagChanged(FGameplayTag Tag, bool bAdded);
	// ~ End of Binding

private:

	TObjectPtr<UGameplayInputCollectionAsset> InputAsset;
	TSharedPtr<FStreamableHandle> AssetHandle;

};

