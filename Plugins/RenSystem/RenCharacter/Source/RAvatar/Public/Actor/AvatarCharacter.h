// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Actor/RCharacterBase.h"
#include "Interface/AssetInstanceData.h"

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
class AAvatarCharacter : public ARCharacterBase, public IAssetInstanceData
{

	GENERATED_BODY()

public:

	AAvatarCharacter();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	TObjectPtr<UCameraComponent> Camera;


	// ~ IAssetInstanceData
	virtual FGuid GetInstanceId() const override;
	virtual void SetInstanceId(const FGuid& InstanceId) override;
	// ~ End of IAssetInstanceData

	// ~ ARCharacterBase
	virtual void InitializeCharacter(const UCharacterAsset* CharacterAsset) override;
	// ~ End of ARCharacterBase

protected:

	UPROPERTY(VisibleAnywhere)
	FGuid AvatarId;

	UPROPERTY()
	TObjectPtr<UAvatarStorage> AvatarStorage;

	UPROPERTY(EditAnywhere)
	float CameraMinZoom = 100.0f;

	UPROPERTY(EditAnywhere)
	float CameraMaxZoom = 400.0f;


	UFUNCTION(BlueprintCallable, Meta = (BlueprintProtected))
	void CameraPan(FVector2D Axis);

	UFUNCTION(BlueprintCallable, Meta = (BlueprintProtected))
	void CameraZoom(float Delta, float Multiplier = 5.0f);

};

