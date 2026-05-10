// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Component/RGameplayEffectContext.h"

// Engine Headers

// Project Headers



FRGameplayEffectContext::FRGameplayEffectContext() : FGameplayEffectContext()
{

}

FRGameplayEffectContext* FRGameplayEffectContext::Resolve(FGameplayEffectContextHandle Handle)
{
	FGameplayEffectContext* Context = Handle.Get();
	if (!Context || !Context->GetScriptStruct()->IsChildOf(FRGameplayEffectContext::StaticStruct()))
	{
		return nullptr;
	}

	return (FRGameplayEffectContext*)Context;
}

UScriptStruct* FRGameplayEffectContext::GetScriptStruct() const
{
	return FRGameplayEffectContext::StaticStruct();
}

FRGameplayEffectContext* FRGameplayEffectContext::Duplicate() const
{
	FRGameplayEffectContext* NewContext = new FRGameplayEffectContext();
	*NewContext = *this;

	if (GetHitResult())
	{
		NewContext->AddHitResult(*GetHitResult(), true);
	}

	return NewContext;
}