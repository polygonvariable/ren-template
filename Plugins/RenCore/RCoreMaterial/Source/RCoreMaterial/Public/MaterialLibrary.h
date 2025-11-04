// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Forward Declarations
class UMaterialParameterCollectionInstance;



/**
 *
 */
class MaterialLibrary
{

public:

	RCOREMATERIAL_API static void LerpScalarParameter(UMaterialParameterCollectionInstance* MPC, FName ParameterName, float Target, float Alpha);
	RCOREMATERIAL_API static void LerpVectorParameter(UMaterialParameterCollectionInstance* MPC, FName ParameterName, const FLinearColor& Target, float Alpha);

};

