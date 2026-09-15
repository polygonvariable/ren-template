// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "MaterialSurfaceProperty.h"

// Module Macros
#define REN_API RCOREMATERIAL_API

// Forward Declarations
class UMaterialParameterCollectionInstance;



/**
 *
 */
namespace FMaterialLibrary
{

	REN_API void GetSurfaceProperty(FMaterialSurfaceProperty& Surface, UMaterialParameterCollectionInstance* Instance, FName TintName, FName SROWName, FName DCMAName);
	REN_API void SetSurfaceProperty(const FMaterialSurfaceProperty& Surface, UMaterialParameterCollectionInstance* Instance, FName TintName, FName SROWName, FName DCMAName);
	REN_API void LerpSurfaceProperty(const FMaterialSurfaceProperty& A, const FMaterialSurfaceProperty& B, float Alpha, FMaterialSurfaceProperty& OutResult);

};



// Module Macros
#undef REN_API

