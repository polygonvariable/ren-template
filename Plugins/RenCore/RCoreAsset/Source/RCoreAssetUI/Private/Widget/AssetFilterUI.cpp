// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/AssetFilterUI.h"

// Engine Headers
#include "Components/Button.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"

// Project Headers
#include "Filter/FilterCriterion.h"
#include "Widget/AssetCollectionUI.h"



void UAssetFilterUI::SetSelected()
{
	OnSelected.ExecuteIfBound();
}

void UAssetFilterUI::NativePreConstruct()
{
	Super::NativePreConstruct();
	FilterTitle->SetText(TitleText);
}

void UAssetFilterUI::NativeConstruct()
{
	FilterButton->OnClicked.AddDynamic(this, &UAssetFilterUI::SetSelected);
	Super::NativeConstruct();
}

void UAssetFilterUI::NativeDestruct()
{
	FilterButton->OnClicked.RemoveAll(this);
	Super::NativeDestruct();
}



void UAssetFilterCollectionUI::SetTargetCollectionUI(UAssetCollectionUI* InCollectionUI)
{
	_AssetCollection = InCollectionUI;
}

void UAssetFilterCollectionUI::NativePreConstruct()
{
	Super::NativePreConstruct();

	FilterBox->ClearChildren();

	int FilterCount = FilterCriteria.Num();

	for (int i = 0; i < FilterCount; i++)
	{
		UAssetFilterUI* FilterUI = CreateWidget<UAssetFilterUI>(this, FilterClass);
		if (IsValid(FilterUI))
		{
			FilterUI->SetPadding(FilterSpacing);
			FilterUI->TitleText = FText::FromString(FilterCriteria[i].FilterName.ToString());
			if (!IsDesignTime())
			{
				FilterUI->OnSelected.BindUObject(this, &UAssetFilterCollectionUI::OnFilterSelected, i);
			}
			FilterBox->AddChild(FilterUI);
		}
	}
}

void UAssetFilterCollectionUI::OnFilterSelected(int Index)
{
	if (!FilterCriteria.IsValidIndex(Index))
	{
		return;
	}
	
	OnSelected.Broadcast(Index);

	const FAssetFilterCriterion& AssetFilter = FilterCriteria[Index];
	const UFilterCriterion* FilterCriterion = AssetFilter.FilterCriterion;
	if (!IsValid(FilterCriterion))
	{
		return;
	}

	UAssetCollectionUI* AssetCollection = _AssetCollection.Get();
	if (!IsValid(AssetCollection))
	{
		return;
	}

	UFilterCriterion* Criterion = AssetCollection->GetCriterionByName(FilterCriterion->GetPropertyName());
	if (!IsValid(Criterion))
	{
		return;
	}

	Criterion->ClearEvaluationData();
	Criterion->CopyEvaluationData(FilterCriterion);
	AssetCollection->RefreshEntries();
}

