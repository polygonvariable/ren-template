// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"

// Project Headers

// Generated Headers
#include "RGameplayEffectContext.generated.h"

// Forward Declarations



/**
 * 
 * 
 * 
 */
USTRUCT()
struct FRGameplayEffectContext : public FGameplayEffectContext
{

	GENERATED_BODY()

public:

	FRGameplayEffectContext();

	static FRGameplayEffectContext* Resolve(FGameplayEffectContextHandle Handle);

	// ~ FGameplayEffectContext
	virtual UScriptStruct* GetScriptStruct() const override;
	virtual FRGameplayEffectContext* Duplicate() const;
	// ~ End of FGameplayEffectContext

};

template<>
struct TStructOpsTypeTraits<FRGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FRGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};

