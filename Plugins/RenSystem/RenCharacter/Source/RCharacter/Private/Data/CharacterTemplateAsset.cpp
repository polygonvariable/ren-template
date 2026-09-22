// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Data/CharacterTemplateAsset.h"

// Engine Headers
#if WITH_EDITOR
#include "GameplayEffect.h"
#include "Misc/DataValidation.h"
#endif


#if WITH_EDITOR
EDataValidationResult UCharacterTemplateAsset::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);
	
	if (!IsValid(InitialAttributeEffect))
	{
		Context.AddError(FText::FromString("Initial attribute effect class is invalid"));
		return EDataValidationResult::Invalid;
	}

	for (const FComponentDefinition& Definition : Components)
	{
		if (!Definition.IsDataValid())
		{
			Context.AddError(FText::FromString("Invalid component definition"));
			return EDataValidationResult::Invalid;
		}
	}

	return Result;
}
#endif

