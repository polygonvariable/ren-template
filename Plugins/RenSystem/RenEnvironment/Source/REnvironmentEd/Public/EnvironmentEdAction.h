// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"

// Project Headers

// Generated Headers

// Forward Declarations


#define ACTION_METHODS_DECLARE() \
	virtual FText GetName() const override; \
	virtual FColor GetTypeColor() const override; \
	virtual UClass* GetSupportedClass() const override; \
	virtual uint32 GetCategories() override;


/**
 *
 *
 */
class FEnvironmentProfileActions : public FAssetTypeActions_Base
{

public:

	ACTION_METHODS_DECLARE()

};


/**
 *
 *
 */
class FEnvironmentFogProfileActions : public FAssetTypeActions_Base
{

public:

	ACTION_METHODS_DECLARE()

};

/**
 *
 *
 */
class FEnvironmentLightProfileActions : public FAssetTypeActions_Base
{

public:

	ACTION_METHODS_DECLARE()

};

/**
 *
 *
 */
class FEnvironmentAtmosphereProfileActions : public FAssetTypeActions_Base
{

public:

	ACTION_METHODS_DECLARE()

};


/**
 *
 *
 */
class FEnvironmentAssetActions : public FAssetTypeActions_Base
{

public:

	ACTION_METHODS_DECLARE()

};

