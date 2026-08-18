// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "System/Controller/EquipmentStateController_Attachment.h"

// Generated Headers
#include "EquipmentStateController_Weapon.generated.h"

// Module Macros
#define REN_API REQUIPMENT_API


/**
 *
 */
UCLASS(Abstract, MinimalAPI, Blueprintable, meta = (DisplayName = "Equipment Controller (Weapon)"))
class UEquipmentWeaponController : public UEquipmentAttachmentController
{

	GENERATED_BODY()

public:

	DECLARE_MULTICAST_DELEGATE(FOnWeaponDataChanged)
	FOnWeaponDataChanged OnDataChanged;

};


/**
 *
 */
UCLASS(Abstract, MinimalAPI, Blueprintable, meta = (DisplayName = "Equipment Controller (Projectile Weapon)"))
class UEquipmentProjectileWeaponController : public UEquipmentWeaponController
{

	GENERATED_BODY()

public:


	UFUNCTION(BlueprintCallable)
	int GetProjectileCount() const
	{
		return ProjectileCount;
	}

	UFUNCTION(BlueprintCallable)
	int GetMaxProjectileCount() const
	{
		return ProjectileCountMax;
	}

	UFUNCTION(BlueprintCallable)
	bool CanReloadProjectiles() const
	{
		return ProjectileCount < ProjectileCountMax;
	}

	UFUNCTION(BlueprintCallable)
	void ReloadProjectiles()
	{
		ProjectileCount = ProjectileCountMax;
		OnDataChanged.Broadcast();
	}

	UFUNCTION(BlueprintCallable)
	int ConsumeProjectile()
	{
		if (ProjectileCount > 0)
		{
			ProjectileCount = FMath::Clamp(ProjectileCount - 1, 0, ProjectileCountMax);
			OnDataChanged.Broadcast();
		}
		return ProjectileCount;
	}

	UFUNCTION(BlueprintCallable)
	bool ConsumeAndCheckProjectiles()
	{
		ConsumeProjectile();
		return HasEnoughProjectiles();
	}

	UFUNCTION(BlueprintCallable)
	bool HasEnoughProjectiles() const
	{
		return ProjectileCount > 0 || HasInfiniteProjectiles();
	}

	bool HasInfiniteProjectiles() const
	{
		return ProjectileCountMax == -1;
	}

	UPROPERTY(EditDefaultsOnly)
	int ProjectileCountMax = 10;

protected:

	int ProjectileCount = 10;

};


// Module Macros
#undef REN_API

