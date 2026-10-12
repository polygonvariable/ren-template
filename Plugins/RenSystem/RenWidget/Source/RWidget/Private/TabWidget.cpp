// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "TabWidget.h"

// Engine Headers
#include "Components/Button.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"



// Project Headers
#include "Log/LogMacro.h"


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
		LOG_ERROR(LogTemp, TEXT("TabControlClass must be child of UTabControl"));
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
		LOG_ERROR(LogTemp, TEXT("TabContainer is not valid"));
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
		LOG_ERROR(LogTemp, TEXT("Tab or TabContainer is not valid"));
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
		LOG_ERROR(LogTemp, TEXT("TabContainer is not valid"));
		return;
	}
	TabContainer->AddChild(NewTab);
}

UCardButton::UCardButton()
{
	LabelText = FText::FromString(TEXT("Button"));

	CardTextPadding = FMargin(16.0f, 3.0f, 14.0f, 2.0f);
	CardSlotPadding = FMargin(12.0f, 4.0f, 4.0f, 4.0f);
	TextSlotPadding = FMargin(0.0f, 4.0f, 12.0f, 4.0f);
}

void UCardButton::BuildDefaultContent()
{
	if (GetChildrenCount() > 0)
	{
		return;
	}

	UHorizontalBox* HorizontalBox = NewObject<UHorizontalBox>(this, TEXT("HorizontalBox001"));
	if (IsValid(HorizontalBox))
	{
		HorizontalBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		CardBorder = NewObject<UBorder>(this, TEXT("CardBorder001"));
		if (IsValid(CardBorder))
		{
			CardBorder->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

			CardTextBlock = NewObject<UTextBlock>(this, TEXT("CardText001"));
			if (IsValid(CardTextBlock))
			{
				CardTextBlock->SetText(CardText);
				CardTextBlock->SetMargin(CardTextPadding);
				CardTextBlock->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

				CardBorder->AddChild(CardTextBlock);
			}

			UHorizontalBoxSlot* CardSlot = HorizontalBox->AddChildToHorizontalBox(CardBorder);
			if (IsValid(CardSlot))
			{
				CardSlot->SetPadding(CardSlotPadding);
				CardSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
			}
		}

		LabelTextBlock = NewObject<UTextBlock>(this, TEXT("Label001"));
		if (IsValid(LabelTextBlock))
		{
			LabelTextBlock->SetText(LabelText);
			LabelTextBlock->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

			UHorizontalBoxSlot* TextSlot = HorizontalBox->AddChildToHorizontalBox(LabelTextBlock);
			if (IsValid(TextSlot))
			{
				TextSlot->SetPadding(TextSlotPadding);
				TextSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
			}
		}

		AddChild(HorizontalBox);
	}
}

TSharedRef<SWidget> UCardButton::RebuildWidget()
{
	BuildDefaultContent();

	TSharedRef<SWidget> ButtonWidget = Super::RebuildWidget();
	return ButtonWidget;
}

void UCardButton::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	
	if (IsValid(ButtonStyle))
	{
		const FButtonStyle* Style = ButtonStyle->GetStyle<FButtonStyle>();
		if (Style)
		{
			SetStyle(*Style);
		}
	}

	if (IsValid(LabelTextBlock))
	{
		LabelTextBlock->SetText(LabelText);

		if (LabelTextStyle)
		{
			const FTextBlockStyle* TextStyle = LabelTextStyle->GetStyle<FTextBlockStyle>();
			if (TextStyle)
			{
				LabelTextBlock->SetFont(TextStyle->Font);
				LabelTextBlock->SetColorAndOpacity(TextStyle->ColorAndOpacity);
			}
		}
	}

	if (IsValid(CardBorder))
	{
		CardBorder->SetBrushFromAsset(CardStyle);
	}

	if (IsValid(CardTextBlock))
	{
		if (CardText.IsEmpty())
		{
			CardTextBlock->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			CardTextBlock->SetText(CardText);
			CardTextBlock->SetMargin(CardTextPadding);
			CardTextBlock->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			if (CardTextStyle)
			{
				const FTextBlockStyle* TextStyle = CardTextStyle->GetStyle<FTextBlockStyle>();
				if (TextStyle)
				{
					CardTextBlock->SetFont(TextStyle->Font);
					CardTextBlock->SetColorAndOpacity(TextStyle->ColorAndOpacity);
				}
			}
		}
	}
}

void UCardButton::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	LabelTextBlock = nullptr;
	CardBorder = nullptr;
	CardTextBlock = nullptr;
}



void UCardContainer::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	
	if (CardStyle)
	{
		SetBrushFromAsset(CardStyle);
	}
}



void UCardItem::IsActive(bool bActive)
{
	if (bActive)
	{
		if (ActiveStyle)
		{
			SetBrushFromAsset(ActiveStyle);
		}
	}
	else
	{
		if (InactiveStyle)
		{
			SetBrushFromAsset(InactiveStyle);
		}
	}
}

void UCardItem::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	
	if (InactiveStyle)
	{
		SetBrushFromAsset(InactiveStyle);
	}
}
