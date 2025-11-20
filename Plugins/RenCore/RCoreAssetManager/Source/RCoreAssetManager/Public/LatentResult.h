// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers

// Generated Headers

// Forward Declarations



/**
 *
 */
struct FLatentResult
{

public:

	bool bSuccess = false;
	bool bCancelled = false;

	/*
	 * Returns true if the following are satisfied:
	 * - bSuccess is true
	 * - bCancelled is false
	 */
	bool IsValid() const
	{
		return bSuccess && !bCancelled;
	}

};

/**
 *
 */
template<typename T>
struct FLatentResultAsset : public FLatentResult
{

public:

	T* Asset = nullptr;

	/*
	 * Returns true if the following conditions are satisfied:
	 * - FLatentResult is valid (bSuccess is true and bCancelled is false)
	 * - Asset pointer is valid
	 */
	bool IsValid() const
	{
		return (bSuccess && !bCancelled) && Asset != nullptr;
	}

};

/**
 *
 */
template<typename T>
struct FLatentResultAssets : public FLatentResult
{

public:

	TArray<T*> Assets;

	/*
	 * Returns true if the following conditions are satisfied:
	 * - FLatentResult is valid (bSuccess is true and bCancelled is false)
	 * - Assets array is not empty
	 */
	bool IsValid() const
	{
		return (bSuccess && !bCancelled) && !Assets.IsEmpty();
	}

	const TArray<T*>& Get() const
	{
		return Assets;
	}

};

