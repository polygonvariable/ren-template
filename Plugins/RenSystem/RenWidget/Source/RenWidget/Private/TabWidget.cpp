// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "TabWidget.h"

// Engine Headers
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"

// Project Headers
#include "RenCoreLibrary/Public/LogMacro.h"


void UTabControl::SetSelected_Implementation(bool bIsSelected)
{
	bSelected = bIsSelected;
}

void UTabControl::HandleSelection_Implementation()
{
	OnSelected.Broadcast(this);
}

void UTabControl::NativeConstruct()
{
	Super::NativeConstruct();
	if (IsValid(TabButton))
	{
		TabButton->OnClicked.AddDynamic(this, &UTabControl::HandleSelection);
	}
	if (IsValid(TabTextBlock))
	{
		TabTextBlock->SetText(TabTitle);
	}
}

void UTabControl::NativeDestruct()
{
	if (IsValid(TabButton))
	{
		TabButton->OnClicked.RemoveAll(this);
	}
	Super::NativeDestruct();
}


void UTabBoxControl::BuildTabs_Implementation(const TMap<uint8, FName>& TabItems)
{
	if(!TabControlClass || !TabControlClass->IsChildOf(UTabControl::StaticClass()))
	{
		LOG_ERROR(LogTemp, "TabControlClass must be child of UTabControl");
		return;
	}
	
	for (auto Item : TabItems)
	{
		UWidget* TabItem = CreateWidget<UTabControl>(this, TabControlClass);
		if (!IsValid(TabItem))
		{
			continue;
		}

		UTabControl* Tab = Cast<UTabControl>(TabItem);
		if (!IsValid(Tab))
		{
			continue;
		}

		Tab->TabIndex = Item.Key;
		Tab->TabTitle = FText::FromString(Item.Value.ToString());
		Tab->OnSelected.RemoveAll(this);
		Tab->OnSelected.AddDynamic(this, &UTabBoxControl::HandleTabSelected);

		HandleTabSetup(Tab);
	}
}

void UTabBoxControl::ClearTabs_Implementation()
{
	if (!IsValid(TabContainer))
	{
		LOG_ERROR(LogTemp, "TabContainer is not valid");
		return;
	}
	TArray<UWidget*> Tabs = TabContainer->GetAllChildren();
	for (UWidget* Tab : Tabs)
	{
		UTabControl* ResolvedTab = Cast<UTabControl>(Tab);
		if (!IsValid(ResolvedTab))
		{
			continue;
		}
		ResolvedTab->OnSelected.RemoveAll(this);
	}
	TabContainer->ClearChildren();
}

void UTabBoxControl::HandleTabSelected_Implementation(UTabControl* SelectedTab)
{
	if (!IsValid(SelectedTab) || !IsValid(TabContainer))
	{
		LOG_ERROR(LogTemp, "Tab or TabContainer is not valid");
		return;
	}

	TArray<UWidget*> Tabs = TabContainer->GetAllChildren();
	for (UWidget* Tab : Tabs)
	{
		UTabControl* ResolvedTab = Cast<UTabControl>(Tab);
		if (!IsValid(ResolvedTab))
		{
			continue;
		}
		ResolvedTab->SetSelected(Tab == SelectedTab);
	}

	OnTabChanged.Broadcast(SelectedTab->TabIndex);
}

void UTabBoxControl::HandleTabSetup_Implementation(UTabControl* NewTab) {
	if (!IsValid(TabContainer))
	{
		LOG_ERROR(LogTemp, "TabContainer is not valid");
		return;
	}
	TabContainer->AddChild(NewTab);
}

