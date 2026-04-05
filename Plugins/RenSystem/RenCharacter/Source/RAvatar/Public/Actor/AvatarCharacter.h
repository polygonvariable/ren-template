// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Actor/CharacterBase.h"
#include "Interface/AssetInstanceContextProvider.h"
#include "Definition/Runtime/AvatarInstance.h"

// Generated Headers
#include "AvatarCharacter.generated.h"

// Forward Declarations
class USpringArmComponent;
class UCameraComponent;
class UCharacterAsset;
class UAvatarStorage;



/**
 *
 * 
 * 
 */
UCLASS(Abstract, MinimalAPI)
class AAvatarCharacter : public ACharacterBase, public IAssetInstanceContextProvider
{

	GENERATED_BODY()

public:

	AAvatarCharacter();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	TObjectPtr<UCameraComponent> Camera;


	// ~ ISpawnContextProvider
	virtual FGuid GetAssetInstanceId() const override;
	virtual FPrimaryAssetId GetAssetId() const override;
	virtual FPrimaryAssetType GetAssetType() const override;
	// ~ End of ISpawnContextProvider

	// ~ ACharacterBase
	virtual void InitializeCharacter() override;
	virtual void DeinitializeCharacter() override;
	// ~ End of ACharacterBase

protected:

	UPROPERTY(VisibleAnywhere)
	FAvatarInstance AvatarInstance;

	UPROPERTY()
	TObjectPtr<UAvatarStorage> AvatarStorage;

	UPROPERTY(EditAnywhere)
	float CameraMinZoom = 100.0f;

	UPROPERTY(EditAnywhere)
	float CameraMaxZoom = 400.0f;


	// ~ ACharacterBase
	virtual void RefreshCharacter() override;
	virtual void AddRuntimeAttributes() override;
	virtual int GetCharacterLevel() const override;
	// ~ End of ACharacterBase


	UFUNCTION(BlueprintCallable, Meta = (BlueprintProtected))
	void CameraPan(FVector2D Axis);

	UFUNCTION(BlueprintCallable, Meta = (BlueprintProtected))
	void CameraZoom(float Delta, float Multiplier = 5.0f);

};

