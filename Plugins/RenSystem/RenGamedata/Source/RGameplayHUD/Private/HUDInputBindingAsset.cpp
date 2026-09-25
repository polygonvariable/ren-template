// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "HUDInputBindingAsset.h"

// Engine Headers
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif
#include "InputAction.h"
#include "UObject/ObjectSaveContext.h"


bool FHUDInputBinding::IsValid() const
{
	return InputId.IsValid() && InputAction != nullptr && !WidgetClass.IsNull();
}

void UHUDInputBindingAsset::PreSave(FObjectPreSaveContext ObjectSaveContext)
{
#if WITH_EDITOR
	for (FHUDInputBinding& Binding : Bindings)
	{
		Binding.InputId = FGuid::NewGuid();
	}
#endif

	Super::PreSave(ObjectSaveContext);
}

#if WITH_EDITOR
EDataValidationResult UHUDInputBindingAsset::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	for (const FHUDInputBinding& Binding : Bindings)
	{
		if (!Binding.IsValid())
		{
			Context.AddError(FText::FromString("Invalid input binding in list"));
			return EDataValidationResult::Invalid;
		}
	}

	return Result;
}
#endif

