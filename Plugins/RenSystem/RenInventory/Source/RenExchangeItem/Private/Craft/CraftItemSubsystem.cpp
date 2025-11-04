// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Craft/CraftItemSubsystem.h"

// Engine Headers

// Project Headers
#include "RCoreCraft/Public/CraftProviderInterface.h"
#include "RCoreExchange/Public/ExchangeRule.h"



const FExchangeRule* UCraftItemSubsystem::GetExchangeRule(UObject* Target) const
{
	ICraftProviderInterface* CraftProvider = Cast<ICraftProviderInterface>(Target);
	if (!CraftProvider)
	{
		return nullptr;
	}

	const FExchangeRule& ExchangeRule = CraftProvider->GetCraftingRule();
	return &ExchangeRule;
}


bool UCraftItemSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

