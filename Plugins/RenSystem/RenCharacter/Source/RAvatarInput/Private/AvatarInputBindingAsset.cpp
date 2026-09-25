// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "AvatarInputBindingAsset.h"

// Engine Headers
#if WITH_EDITOR
#include "InputAction.h"
#include "Misc/DataValidation.h"
#endif


#if WITH_EDITOR
EDataValidationResult UAvatarInputBindingAsset::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	if (!IsValid(CameraPan))
	{
		Context.AddError(FText::FromString("Camera pan input action is invalid"));
		return EDataValidationResult::Invalid;
	}

	if (!IsValid(CameraZoom))
	{
		Context.AddError(FText::FromString("Camera zoom input action is invalid"));
		return EDataValidationResult::Invalid;
	}

	if (!IsValid(Walk))
	{
		Context.AddError(FText::FromString("Walk input action is invalid"));
		return EDataValidationResult::Invalid;
	}

	if (!IsValid(Sprint))
	{
		Context.AddError(FText::FromString("Sprint input action is invalid"));
		return EDataValidationResult::Invalid;
	}

	if (!IsValid(Jump))
	{
		Context.AddError(FText::FromString("Jump input action is invalid"));
		return EDataValidationResult::Invalid;
	}

	return Result;
}
#endif

