// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Generated Headers
#include "AvatarInputBindingComponent.generated.h"

// Forward Declarations
class AAvatarCharacter;
class UEnhancedInputComponent;
class UAvatarInputBindingAsset;
struct FInputActionValue;
struct FStreamableHandle;


/**
 *
 */
UCLASS(MinimalAPI, meta = (BlueprintSpawnableComponent))
class UAvatarInputBindingComponent : public UActorComponent
{

	GENERATED_BODY()

public:

	UAvatarInputBindingComponent();

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
	TSoftObjectPtr<UAvatarInputBindingAsset> InputBinding;

	UPROPERTY()
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY()
	TObjectPtr<AAvatarCharacter> Character;


	UEnhancedInputComponent* GetInputComponent() const;

	// ~ Binding
	UFUNCTION()
	void HandleOnPawnChanged(APawn* OldPawn, APawn* NewPawn);
	void HandleOnInputAssetLoaded();
	// ~ End of Binding

	// ~ Input Binding
	void HandleOnCameraPan(const FInputActionValue& Value);
	void HandleOnCameraZoom(const FInputActionValue& Value);
	void HandleOnWalk(const FInputActionValue& Value);
	void HandleOnSprint(const FInputActionValue& Value);
	void HandleOnJump(const FInputActionValue& Value);
	// ~ End of Input Binding

private:

	UPROPERTY()
	TObjectPtr<UAvatarInputBindingAsset> InputBindingAsset;

	TArray<uint8> InputHandles;
	TSharedPtr<FStreamableHandle> AssetHandle;

};
