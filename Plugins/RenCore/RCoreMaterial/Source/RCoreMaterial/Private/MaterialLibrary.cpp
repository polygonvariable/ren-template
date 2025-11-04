// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "MaterialLibrary.h"

// Engine Headers
#include "Materials/MaterialParameterCollectionInstance.h"

// Project Headers



void MaterialLibrary::LerpScalarParameter(UMaterialParameterCollectionInstance* MPC, FName ParameterName, float Target, float Alpha)
{
	if (!MPC)
	{
		return;
	}

	float Current = 0.0f;
	bool bFound = MPC->GetScalarParameterValue(ParameterName, Current);
	if (bFound)
	{
		MPC->SetScalarParameterValue(ParameterName, FMath::InterpEaseInOut(Current, Target, Alpha, 3.0f));
	}
}

void MaterialLibrary::LerpVectorParameter(UMaterialParameterCollectionInstance* MPC, FName ParameterName, const FLinearColor& Target, float Alpha)
{
	if (!MPC)
	{
		return;
	}

	FLinearColor Current = FLinearColor::Transparent;
	bool bFound = MPC->GetVectorParameterValue(ParameterName, Current);
	if (bFound)
	{
		MPC->SetVectorParameterValue(ParameterName, FMath::InterpEaseInOut(Current, Target, Alpha, 3.0f));
	}
}

