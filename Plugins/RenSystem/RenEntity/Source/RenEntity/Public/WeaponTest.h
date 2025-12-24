// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// Generated Headers
#include "WeaponTest.generated.h"

// Forward Declarations
class UAbilitySystemComponent;
class UGameplayEffect;

/**
 *
 */
UCLASS()
class AWeaponTest : public AActor
{

	GENERATED_BODY()
	
public:

	AWeaponTest();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

protected:

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};

