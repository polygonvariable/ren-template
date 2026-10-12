// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "WidgetTemplateDefinition.h"

// Engine Headers
#include "Blueprint/UserWidget.h"


UUserWidget* FWidgetTemplateDefinition::CreateWidget(UWorld* World)
{
	UClass* WidgetClass = GetWidgetClass();
	checkf(IsValid(World), TEXT("Widget template was provided with invalid world"));
	checkf(IsValid(WidgetClass), TEXT("Widget template contains invalid widget class or is not loaded into memory"));

	UUserWidget* Widget = ::CreateWidget<UUserWidget>(World, WidgetClass);
	checkf(IsValid(Widget), TEXT("Failed to create new widget using template"));

	InitializeInstance(Widget);

	return Widget;
}

#if WITH_EDITOR
bool FWidgetTemplateDefinition::IsDataValid() const
{
	return false;
}
void FWidgetTemplateDefinition::AppendAssetBundleData(FAssetBundleData& AssetBundle, const FName& Bundle)
{
}
#endif

TSubclassOf<UUserWidget> FWidgetTemplateDefinition::GetWidgetClass() const
{
	return nullptr;
}

void FWidgetTemplateDefinition::InitializeInstance(UUserWidget* Widget)
{
}

