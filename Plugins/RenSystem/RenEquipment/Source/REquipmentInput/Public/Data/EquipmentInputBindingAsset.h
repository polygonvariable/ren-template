// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif
#include "Engine/DataAsset.h"

// Project Headers
#include "Core/Type/EquipmentInputBinding.h"

// Generated Headers
#include "EquipmentInputBindingAsset.generated.h"

// Forward Declarations
class UInputAction;
class UEquipmentActivationInput;
class UEquipmentActionInput;


/**
 *
 */
UCLASS(MinimalAPI)
class UEquipmentInputBindingAsset : public UPrimaryDataAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Equipment Activation")
	TArray<FEquipmentActivationBinding> ActivationBindings;

	UPROPERTY(EditAnywhere, Category = "Equipment Activation")
	TSubclassOf<UEquipmentActivationInput> ActivationClass;

	UPROPERTY(EditAnywhere, Category = "Equipment Triggers")
	TArray<FEquipmentActionBinding> ActionBindings;

	UPROPERTY(EditAnywhere, Category = "Equipment Triggers")
	TSubclassOf<UEquipmentActionInput> ActionClass;


#if WITH_EDITOR
	// ~ UPrimaryDataAsset
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override
	{
		EDataValidationResult Result = Super::IsDataValid(Context);

		if (ActivationBindings.Num() > 0)
		{
			for (const FEquipmentActivationBinding& ActivationBinding : ActivationBindings)
			{
				if (!ActivationBinding.IsValid())
				{
					Context.AddError(FText::FromString("Invalid activation binding in list"));
					return EDataValidationResult::Invalid;
				}
			}

			if (!IsValid(ActivationClass))
			{
				Context.AddError(FText::FromString("Activation class is invalid"));
				return EDataValidationResult::Invalid;
			}
		}

		if (ActionBindings.Num() > 0)
		{
			for (const FEquipmentActionBinding& ActionBinding : ActionBindings)
			{
				if (!ActionBinding.IsValid())
				{
					Context.AddError(FText::FromString("Invalid action binding in list"));
					return EDataValidationResult::Invalid;
				}
			}

			if (!IsValid(ActionClass))
			{
				Context.AddError(FText::FromString("Action class is invalid"));
				return EDataValidationResult::Invalid;
			}
		}

		return Result;
	}
	// ~ End of UPrimaryDataAsset
#endif

};

