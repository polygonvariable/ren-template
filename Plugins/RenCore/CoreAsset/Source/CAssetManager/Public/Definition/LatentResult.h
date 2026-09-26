// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


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

	bool IsCompleted() const
	{
		return bSuccess && !bCancelled;
	}

};

/**
 *
 */
template<typename T>
struct FLatentLoadedAsset : public FLatentResult
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

	const T* Get() const
	{
		return Asset;
	}
};

/**
 *
 */
template<typename T>
struct FLatentLoadedAssets : public FLatentResult
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

