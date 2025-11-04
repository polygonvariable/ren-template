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
struct FLatentHandle
{

public:

	void Cancel()
	{
		bIsCancelled = true;
	}

	bool IsCancelled() const
	{
		return bIsCancelled;
	}

private:

	std::atomic<bool> bIsCancelled{ false };

};

