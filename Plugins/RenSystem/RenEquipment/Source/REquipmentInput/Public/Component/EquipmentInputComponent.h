// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Core/Type/EquipmentSlotId.h"

// Generated Headers
#include "EquipmentInputComponent.generated.h"

// Forward Declarations
class UEquipmentInputHandler;
struct FStreamableHandle;
struct FInputActionValue;

/**
 *
 */
UCLASS(MinimalAPI, meta = (BlueprintSpawnableComponent))
class UEquipmentInputComponent : public UActorComponent
{

	GENERATED_BODY()

public:

	UEquipmentInputComponent(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, meta = (MetaClass = "/Script/REquipment.EquipmentInputMapping"))
	TArray<FSoftObjectPath> InputMapping;
	

	// ~ UActorComponent
	virtual void Activate(bool bReset) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// ~ End of UActorComponent

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

	TSharedPtr<FStreamableHandle> _LoadHandle = nullptr;

};

