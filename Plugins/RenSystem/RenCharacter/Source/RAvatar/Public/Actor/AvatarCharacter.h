// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Actor/CharacterBase.h"
#include "Core/Type/Runtime/AvatarInstance.h"
#include "Core/Interface/AssetInstanceContextProvider.h"

// Generated Headers
#include "AvatarCharacter.generated.h"

// Forward Declarations
class USpringArmComponent;
class UCameraComponent;
class UCharacterAsset;
class UAvatarStorageManager;


/**
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

	UPROPERTY(EditAnywhere)
	EDataSource SourceType = EDataSource::Static;


	// ~ IAssetInstanceContextProvider
	virtual FGuid GetAssetInstanceId() const override;
	virtual FPrimaryAssetId GetAssetId() const override;
	virtual FPrimaryAssetType GetAssetType() const override;
	// ~ End of IAssetInstanceContextProvider

	// ~ ISpawnContextProvider
	virtual EDataSource GetSpawnSource() const override;
	// ~ End of ISpawnContextProvider

	// ~ ACharacterBase
	virtual void InitializeCharacter() override;
	virtual void DeinitializeCharacter() override;
	// ~ End of ACharacterBase

protected:

	UPROPERTY(VisibleAnywhere)
	FAvatarInstance AvatarInstance;

	UPROPERTY()
	TObjectPtr<UAvatarStorageManager> StorageManager;

	UPROPERTY(EditAnywhere)
	float CameraMinZoom = 100.0f;

	UPROPERTY(EditAnywhere)
	float CameraMaxZoom = 400.0f;


	// ~ ACharacterBase
	virtual void RefreshCharacter() override;
	virtual void AddRuntimeAttributes() override;
	// ~ End of ACharacterBase


	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected))
	void CameraPan(FVector2D Axis);

	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected))
	void CameraZoom(float Delta, float Multiplier = 5.0f);

};

