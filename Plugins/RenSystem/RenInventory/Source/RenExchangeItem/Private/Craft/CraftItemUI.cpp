// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Craft/CraftItemUI.h"

// Engine Headers

// Project Headers
#include "RCoreCraft/Public/CraftProviderInterface.h"
#include "RenExchangeItem/Public/Craft/CraftItemSubsystem.h"



int UCraftItemUI::GetExchangeQuantity() const
{
	return 1;
}

UExchangeItemSubsystem* UCraftItemUI::GetExchangeItemSubsystem() const
{
	return CraftItemSubsystem.Get();
}

const FExchangeRule* UCraftItemUI::GetExchangeRule(UObject* Target) const
{
	ICraftProviderInterface* CraftProvider = Cast<ICraftProviderInterface>(Target);
	if (!CraftProvider)
	{
		return nullptr;
	}

	const FExchangeRule& ExchangeRule = CraftProvider->GetCraftingRule();
	return &ExchangeRule;
}


void UCraftItemUI::NativeConstruct()
{
	UGameInstance* GameInstance = GetGameInstance();
	if (IsValid(GameInstance))
	{
		UCraftItemSubsystem* CraftItem = GameInstance->GetSubsystem<UCraftItemSubsystem>();
		if (IsValid(CraftItem))
		{
			CraftItem->OnItemExchanged.AddUObject(this, &UCraftItemUI::HandleItemExchanged);
			CraftItemSubsystem = CraftItem;
		}
	}

	Super::NativeConstruct();
}

void UCraftItemUI::NativeDestruct()
{
	UCraftItemSubsystem* CraftItem = CraftItemSubsystem.Get();
	if (IsValid(CraftItem))
	{
		CraftItem->OnItemExchanged.RemoveAll(this);
	}
	CraftItemSubsystem.Reset();

	Super::NativeDestruct();
}

