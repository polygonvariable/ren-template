// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Module Macros
#define REN_API RCOREMATERIAL_API

// Forward Declarations
class UMaterialParameterCollectionInstance;



/**
 *
 */
class MaterialLibrary
{

public:

	REN_API static void LerpScalarParameter(UMaterialParameterCollectionInstance* MPC, FName ParameterName, float Target, float Alpha);
	REN_API static void LerpVectorParameter(UMaterialParameterCollectionInstance* MPC, FName ParameterName, const FLinearColor& Target, float Alpha);

};



// Module Macros
#undef REN_API

