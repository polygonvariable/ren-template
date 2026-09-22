// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DataAsset.h"

// Project Headers
#include "Core/Type/ComponentDefinition.h"

// Generated Headers
#include "CharacterTemplateAsset.generated.h"

// Forward Declarations
class UGameplayEffect;


/**
 * 
 */
UCLASS(MinimalAPI)
class UCharacterTemplateAsset : public UPrimaryDataAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Ability")
	TSubclassOf<UGameplayEffect> InitialAttributeEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
	TArray<FComponentDefinition> Components;


#if WITH_EDITORONLY_DATA
	// ~ UPrimaryDataAsset
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
	// ~ End of UPrimaryDataAsset
#endif

};

