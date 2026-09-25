// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Generated Headers
#include "EquipmentInputBindingComponent.generated.h"

// Forward Declarations
class UEquipmentInputHandler;
struct FStreamableHandle;


/**
 *
 */
UCLASS(MinimalAPI, meta = (BlueprintSpawnableComponent))
class UEquipmentInputBindingComponent : public UActorComponent
{

	GENERATED_BODY()

public:

	UEquipmentInputBindingComponent(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, meta = (MetaClass = "/Script/REquipmentInput.EquipmentInputBindingAsset"))
	TArray<FSoftObjectPath> InputBindings;
	

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

	UPROPERTY(VisibleAnywhere, AdvancedDisplay)
	TArray<TObjectPtr<UEquipmentInputHandler>> InputHandlers;


	void LoadAndRegisterInput();
	void RegisterInput();
	void UnregisterInput();

	// ~ Bindings
	void HandleOnInputLoaded();
	// ~ End of Bindings

private:

	TSharedPtr<FStreamableHandle> AssetHandle = nullptr;

};

