// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "MaterialSurfaceProperty.h"

// Forward Declarations
class UMaterialParameterCollectionInstance;


/**
 *
 */
namespace FMaterialLibrary
{

	CMATERIAL_API void GetSurfaceProperty(FMaterialSurfaceProperty& Surface, UMaterialParameterCollectionInstance* Instance, FName TintName, FName SROWName, FName DCMAName);
	CMATERIAL_API void SetSurfaceProperty(const FMaterialSurfaceProperty& Surface, UMaterialParameterCollectionInstance* Instance, FName TintName, FName SROWName, FName DCMAName);
	CMATERIAL_API void LerpSurfaceProperty(const FMaterialSurfaceProperty& A, const FMaterialSurfaceProperty& B, float Alpha, FMaterialSurfaceProperty& OutResult);

};

