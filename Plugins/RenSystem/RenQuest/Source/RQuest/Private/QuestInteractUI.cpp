// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "QuestInteractUI.h"

// Engine Headers
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif
#include "Components/Button.h"
#include "Components/Overlay.h"
#include "Core/AssetInstanceLibrary.h"
#include "Core/Interface/AssetInstanceCollection.h"
#include "Core/Type/AssetDetail.h"
#include "Core/Type/AssetFilterProperty.h"
#include "Criterion/FilterCriterion_Leaf.h"
#include "Widget/AssetCollectionUI.h"


void UQuestInteractUI::InteractionComplete(EFSMResult Result)
{
	OnInteractionCompleted.ExecuteIfBound(Result);
}

void UQuestInteractUI::NativeConstruct()
{
	Super::NativeConstruct();
}

void UQuestInteractUI::NativeDestruct()
{
	Super::NativeDestruct();
}


UQuestItemExchangeUI::UQuestItemExchangeUI()
{
	SetIsFocusable(true);

	ConfirmKey = EKeys::F;
	CancelKey = EKeys::C;
}

#if WITH_EDITOR
EDataValidationResult UQuestItemExchangeUI::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	if (!ConfirmKey.IsValid())
	{
		Context.AddWarning(FText::FromString("Confirm key is invalid, key event wont be handled"));
	}
	if (!CancelKey.IsValid())
	{
		Context.AddWarning(FText::FromString("Cancel key is invalid, key event wont be handled"));
	}

	if (IsValid(ExchangeCollection))
	{
		const TInstancedStruct<FFilterCriterion>* FilterCriterion = ExchangeCollection->FilterRule.GetCriterionByName(FAssetFilterProperty::AssetId);
		if (!FilterCriterion)
		{
			Context.AddWarning(FText::FromString("Exchange collection have invalid filter criterion, (required type: FilterCriterion_Asset)"));
			return Result;
		}

		const FFilterCriterion_Asset* AssetCriterion = FilterCriterion->GetPtr<FFilterCriterion_Asset>();
		if (!FilterCriterion)
		{
			Context.AddWarning(FText::FromString("Exchange collection's filter criterion is not of type FilterCriterion_Asset"));
			return Result;
		}
	}

	return Result;
}
#endif

void UQuestItemExchangeUI::NativePreConstruct()
{
	Super::NativePreConstruct();

	FFilterCriterion_Asset& AssetCriterion = ExchangeCollection->FilterRule.CriterionRoot.InitializeAs<FFilterCriterion_Asset>();
	AssetCriterion.PropertyName = FAssetFilterProperty::AssetId;
}

void UQuestItemExchangeUI::NativeConstruct()
{
	Super::NativeConstruct();

	ExchangeCollection->PrimarySourceId = InventorySourceId;
	InventoryCollection->PrimarySourceId = InventorySourceId;

	MessageOverlay->SetVisibility(ESlateVisibility::Collapsed);

	RegisterDelegate();
	ApplyFilter();
	DisplayItems();
}

void UQuestItemExchangeUI::NativeDestruct()
{
	UnregisterDelegate();

	Super::NativeDestruct();
}

FReply UQuestItemExchangeUI::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == ConfirmKey)
	{
		HandleOnConfirmClicked();
		return FReply::Handled();
	}
	else if (InKeyEvent.GetKey() == CancelKey)
	{
		HandleOnCancelClicked();
		return FReply::Handled();
	}
	return FReply::Unhandled();
}


void UQuestItemExchangeUI::RegisterDelegate()
{
	ConfirmButton->OnClicked.AddDynamic(this, &UQuestItemExchangeUI::HandleOnConfirmClicked);
	CancelButton->OnClicked.AddDynamic(this, &UQuestItemExchangeUI::HandleOnCancelClicked);
}

void UQuestItemExchangeUI::UnregisterDelegate()
{
	ConfirmButton->OnClicked.Clear();
	CancelButton->OnClicked.Clear();
}

void UQuestItemExchangeUI::ApplyFilter()
{
	TInstancedStruct<FFilterCriterion>* FilterCriterion = ExchangeCollection->FilterRule.GetCriterionByName(FAssetFilterProperty::AssetId);
	if (!ensureAlwaysMsgf(FilterCriterion, TEXT("Failed to find criterion by name")))
	{
		return;
	}

	FFilterCriterion_Asset* AssetCriterion = FilterCriterion->GetMutablePtr<FFilterCriterion_Asset>();
	if (!ensureAlwaysMsgf(AssetCriterion, TEXT("Failed to cast criterion")))
	{
		return;
	}

	AssetCriterion->Included.Empty();
	InventoryItems.GetKeys(AssetCriterion->Included);

	for (const TPair<FPrimaryAssetId, int>& Kv : InventoryItems)
	{
		FAssetDetail AssetDetail;
		AssetDetail.Quantity = Kv.Value;
		ExchangeCollection->AddSubDetails(Kv.Key, FInstancedStruct::Make(AssetDetail));
	}
}

void UQuestItemExchangeUI::DisplayItems()
{
	InventoryCollection->InitializeCollection();
	InventoryCollection->DisplayEntries();

	ExchangeCollection->InitializeCollection();
	ExchangeCollection->DisplayEntries();
}


void UQuestItemExchangeUI::HandleOnConfirmClicked()
{
	IAssetInstanceCollection* InstanceCollection = FAssetInstanceLibrary::GetInstanceCollection(GetGameInstance(), InventoryAssetType, InventorySourceId);
	if (!InstanceCollection)
	{
		ensureAlwaysMsgf(false, TEXT("Failed to get asset instance collection for inventory"));
		return;
	}

	if (InstanceCollection->ContainInstances(InventoryItems, 1))
	{
		InteractionComplete(EFSMResult::Success);
	}
	else
	{
		MessageOverlay->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UQuestItemExchangeUI::HandleOnCancelClicked()
{
	InteractionComplete(EFSMResult::Cancelled);
}






#if WITH_EDITOR
bool FWidgetTemplate_QuestItemExchange::IsDataValid() const
{
	if (WidgetClass.IsNull() || SourceId.IsNone() || !AssetType.IsValid() || Items.IsEmpty())
	{
		return false;
	}
	return true;
}
void FWidgetTemplate_QuestItemExchange::AppendAssetBundleData(FAssetBundleData& AssetBundle, const FName& Bundle)
{
	if (!WidgetClass.IsNull())
	{
		AssetBundle.AddBundleAsset(Bundle, WidgetClass.ToSoftObjectPath().GetAssetPath());
	}
}
#endif

TSubclassOf<UUserWidget> FWidgetTemplate_QuestItemExchange::GetWidgetClass() const
{
	return WidgetClass.Get();
}

void FWidgetTemplate_QuestItemExchange::InitializeInstance(UUserWidget* Widget)
{
	UQuestItemExchangeUI* QuestUI = CastChecked<UQuestItemExchangeUI>(Widget);
	QuestUI->InventorySourceId = SourceId;
	QuestUI->InventoryAssetType = AssetType;
	QuestUI->InventoryItems = Items;
}

