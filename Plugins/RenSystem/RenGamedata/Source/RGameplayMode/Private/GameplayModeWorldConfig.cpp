// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "GameplayModeWorldConfig.h"

// Engine Headers
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif


#if WITH_EDITOR
EDataValidationResult UGameplayModeWorldConfig::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	if (bEnabled)
	{
		if (DefaultMode.IsEmpty())
		{
			Context.AddError(FText::FromString("Default gameplay mode is empty"));
			return EDataValidationResult::Invalid;
		}
	}

	return Result;
}
#endif

