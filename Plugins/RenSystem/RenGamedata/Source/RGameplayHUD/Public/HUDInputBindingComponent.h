// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameplayTagContainer.h"

// Generated Headers
#include "HUDInputBindingComponent.generated.h"

// Forward Declarations
class UEnhancedInputComponent;
class IGameplayModeProvider;
class UHUDInputBindingAsset;
class UUserWidget;
struct FInputActionValue;
struct FStreamableHandle;


/**
 *
 */
UCLASS(MinimalAPI, meta = (BlueprintSpawnableComponent))
class UHUDInputBindingComponent : public UActorComponent
{

	GENERATED_BODY()

public:

	UHUDInputBindingComponent();

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
	TSoftObjectPtr<UHUDInputBindingAsset> InputBinding;

	IGameplayModeProvider* GameplayMode;

	TArray<TPair<FGuid, TObjectPtr<UUserWidget>>> WidgetCollection;


	APlayerController* GetPlayerController() const;
	UEnhancedInputComponent* GetInputComponent() const;

	// ~ Binding
	void HandleOnAssetLoaded();
	void HandleOnInputTriggered(const FInputActionValue& Value, FGuid InputId, FSoftObjectPath Widget);
	void HandleOnWidgetLoaded(FGuid InputId, FSoftObjectPath Widget);

	UFUNCTION()
	void HandleOnWidgetVisibilityChanged(ESlateVisibility Visiblity);
	// ~ End of Binding

private:

	UPROPERTY()
	TObjectPtr<UHUDInputBindingAsset> InputBindingAsset;
	TSharedPtr<FStreamableHandle> AssetHandle;
	TSharedPtr<FStreamableHandle> WidgetHandle;
	TArray<uint8> InputHandles;

};

