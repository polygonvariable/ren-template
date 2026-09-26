// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "WorldFragmentSettings.h"

// Engine Headers
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

// Project Headers
#include "WorldConfigAsset.h"


const UWorldConfigAsset* AWorldFragmentSettings::FindConfigByClass(TSubclassOf<UWorldConfigAsset> InClass) const
{
	for (const TObjectPtr<UWorldConfigAsset>& Config : Configs)
	{
		if (IsValid(Config) && Config->IsA(InClass))
		{
			return Config.Get();
		}
	}
	return nullptr;
}

#if WITH_EDITOR
EDataValidationResult AWorldFragmentSettings::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	for (UWorldConfigAsset* Config : Configs)
	{
		if (!IsValid(Config))
		{
			Context.AddError(FText::FromString("Invalid world config item in list"));
			return EDataValidationResult::Invalid;
		}

		EDataValidationResult ConfigResult = Config->IsDataValid(Context);
		if (ConfigResult == EDataValidationResult::Invalid)
		{
			Context.AddError(FText::FromString("Invalid world config asset"));
			return EDataValidationResult::Invalid;
		}
	}

	return Result;
}
#endif

