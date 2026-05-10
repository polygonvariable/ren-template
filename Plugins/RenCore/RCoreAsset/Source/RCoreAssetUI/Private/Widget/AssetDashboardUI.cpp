// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/AssetDashboardUI.h"

// Engine Headers
#include "Components/Button.h"
#include "Components/Overlay.h"


void UAssetDashboardUI::RedirectToWidget(TSubclassOf<UAssetDashboardUI> WidgetClass)
{

}

void UAssetDashboardUI::InitializeAssetDetail(const UCoreDataAsset* Asset)
{
	Super::InitializeAssetDetail(Asset);

	TArray<UWidget*> Widgets;
	GetAssetWidgets(Widgets);

	for (UWidget* Widget : Widgets)
	{
		IAssetWidget* AssetWidget = Cast<IAssetWidget>(Widget);
		if (AssetWidget)
		{
			AssetWidget->InitializeAssetDetail(Asset);
		}
	}
}

void UAssetDashboardUI::InitializeEntryDetail(const UAssetEntry* Entry)
{
	Super::InitializeEntryDetail(Entry);

	//TArray<UWidget*> Widgets;
	//GetAssetWidgets(Widgets);

	//for (UWidget* Widget : Widgets)
	//{
	//	IAssetWidget* AssetWidget = Cast<IAssetWidget>(Widget);
	//	if (AssetWidget)
	//	{
	//		AssetWidget->InitializeEntryDetail(Entry);
	//	}
	//}
}

void UAssetDashboardUI::GetAssetWidgets_Implementation(TArray<UWidget*>& Widgets)
{

}

void UAssetDashboardUI::LockControls_Implementation()
{
	if (IsValid(LoaderOverlay))
	{
		LoaderOverlay->SetVisibility(ESlateVisibility::HitTestInvisible);

		TArray<UWidget*> Widgets = GetLockingControls();
		for (UWidget* Widget : Widgets)
		{
			if (IsValid(Widget))
			{
				Widget->SetIsEnabled(false);
			}
		}
	}
}

void UAssetDashboardUI::UnlockControls_Implementation()
{
	if (IsValid(LoaderOverlay))
	{
		LoaderOverlay->SetVisibility(ESlateVisibility::Collapsed);

		TArray<UWidget*> Widgets = GetLockingControls();
		for (UWidget* Widget : Widgets)
		{
			if (IsValid(Widget))
			{
				Widget->SetIsEnabled(true);
			}
		}
	}
}

void UAssetDashboardUI::NativeConstruct()
{
	if (IsValid(CloseButton)) CloseButton->OnClicked.AddDynamic(this, &UAssetDashboardUI::CloseWidget);

	Super::NativeConstruct();
}

void UAssetDashboardUI::NativeDestruct()
{
	if (IsValid(CloseButton)) CloseButton->OnClicked.RemoveAll(this);

	Super::NativeDestruct();
}

