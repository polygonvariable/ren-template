// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Actor/QuestObjectiveActor.h"

#include "Components/ShapeComponent.h"

// Project Headers
#include "EventflowTask.h"
#include "GameplayContextInterface.h"








AQuestObjectiveMarker::AQuestObjectiveMarker()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

void AQuestObjectiveMarker::HandlePlayerEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (DoesCollidedWithPlayer(OtherActor) && !bPlayerInRegion)
	{
		OnInteractionCompleted.ExecuteIfBound(EFSMResult::Success);
		bPlayerInRegion = true;
	}
}


bool FGameplayContext_QuestInteractionHandle::Execute(UWorld* World, UObject* Owner, UObject* Instigator)
{
	UActorComponent* Component = Cast<UActorComponent>(Owner);
	if (!::IsValid(Component))
	{
		return false;
	}

	AQuestInteractionMarker* InteractActor = Cast<AQuestInteractionMarker>(Component->GetOwner());
	if (!::IsValid(Component))
	{
		return false;
	}

	InteractActor->OnInteractionCompleted.ExecuteIfBound(EFSMResult::Success);
	return true;
}







UPrimitiveComponent* AObjectiveCascadeMarker::GetCollisionComponent_Implementation() const
{
	return FindComponentByClass<UShapeComponent>();
}

void AObjectiveCascadeMarker::InitialMarkerLocation()
{
	UPrimitiveComponent* Component = GetCollisionComponent();
	if (IsValid(Component) && Locations.IsValidIndex(Index))
	{
		Component->SetWorldLocation(Locations[Index]);
	}
}

void AObjectiveCascadeMarker::UpdateMarkerLocation()
{
	Index++;
	if (!Locations.IsValidIndex(Index))
	{
		OnInteractionCompleted.ExecuteIfBound(EFSMResult::Success);
		return;
	}

	UPrimitiveComponent* Component = GetCollisionComponent();
	if (IsValid(Component))
	{
		Component->SetWorldLocation(Locations[Index]);
	}
}

void AObjectiveCascadeMarker::HandlePlayerEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (DoesCollidedWithPlayer(OtherActor) && !bPlayerInRegion)
	{
		UpdateMarkerLocation();
		bPlayerInRegion = true;
	}
}

void AObjectiveCascadeMarker::HandlePlayerExited(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex)
{
	if (DoesCollidedWithPlayer(OtherActor) && bPlayerInRegion)
	{
		bPlayerInRegion = false;
	}
}

void AObjectiveCascadeMarker::BeginPlay()
{
	Super::BeginPlay();

	InitialMarkerLocation();
}









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








#include "Components/Border.h"
#include "Components/Overlay.h"
#include "Components/Button.h"
#include "Widget/AssetCollectionUI.h"
#include "Core/AssetInstanceLibrary.h"
#include "Core/Interface/AssetInstanceCollection.h"
#include "Core/Interface/AssetInstanceCollectionProvider.h"
#include "Core/Interface/AssetInstanceContextProvider.h"
#include "Criterion/FilterCriterion_Leaf.h"
#include "Core/Type/AssetFilterProperty.h"

UQuestInteractInventoryUI::UQuestInteractInventoryUI()
{
	InventoryAssetType = TEXT("Inventory");
	InventorySourceId = TEXT("inventory001");
}

void UQuestInteractInventoryUI::HandleOnConfirmClicked()
{
	IAssetInstanceCollection* InstanceCollection = FAssetInstanceLibrary::GetInstanceCollection(GetGameInstance(), InventoryAssetType, InventorySourceId);
	if (InstanceCollection->ContainInstances(InventoryItems, 1))
	{
		InteractionComplete(EFSMResult::Success);
	}
	else
	{
		MessageOverlay->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UQuestInteractInventoryUI::HandleOnCancelClicked()
{
	InteractionComplete(EFSMResult::Cancelled);
}

void UQuestInteractInventoryUI::NativePreConstruct()
{
	Super::NativePreConstruct();

	InventoryCollection->PrimarySourceId = InventorySourceId;

	FFilterCriterion_Asset& AssetCriterion = ItemsRequiredCollection->FilterRule.CriterionRoot.InitializeAs<FFilterCriterion_Asset>();
	AssetCriterion.PropertyName = FAssetFilterProperty::AssetId;
}

void UQuestInteractInventoryUI::NativeConstruct()
{
	 Super::NativeConstruct();

	 MessageOverlay->SetVisibility(ESlateVisibility::Collapsed);

	 ConfirmButton->OnClicked.AddDynamic(this, &UQuestInteractInventoryUI::HandleOnConfirmClicked);
	 CancelButton->OnClicked.AddDynamic(this, &UQuestInteractInventoryUI::HandleOnCancelClicked);

	 TInstancedStruct<FFilterCriterion>* FilterCriterion = ItemsRequiredCollection->FilterRule.GetCriterionByName(FAssetFilterProperty::AssetId);
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
	 	ItemsRequiredCollection->AddSubDetails(Kv.Key, FInstancedStruct::Make(AssetDetail));
	 }

	 InventoryCollection->InitializeCollection();
	 InventoryCollection->DisplayEntries();

	 ItemsRequiredCollection->InitializeCollection();
	 ItemsRequiredCollection->DisplayEntries();
}

void UQuestInteractInventoryUI::NativeDestruct()
{
	ConfirmButton->OnClicked.Clear();
	CancelButton->OnClicked.Clear();

	Super::NativeDestruct();
}



#include "Blueprint/UserWidget.h"




bool FInstancedTemplateDefinition::IsDataValid() const
{
	return false;
}

#if WITH_EDITOR
void FInstancedTemplateDefinition::AppendAssetBundleData(FAssetBundleData& AssetBundle, const FName& Bundle)
{
}
#endif



TSubclassOf<UUserWidget> FWidgetTemplateDefinition::GetWidgetClass() const
{
	return nullptr;
}

UUserWidget* FWidgetTemplateDefinition::CreateWidget(UWorld* World)
{
	UClass* WidgetClass = GetWidgetClass();
	checkf(IsValid(WidgetClass), TEXT("Widget template contains invalid widget class"));
	checkf(IsValid(World), TEXT("Widget template was provided with invalid world"));

	UUserWidget* Widget = ::CreateWidget<UUserWidget>(World, WidgetClass);
	checkf(IsValid(Widget), TEXT("Failed to create new widget using template"));

	CopyToInstance(Widget);

	return Widget;
}

void FWidgetTemplateDefinition::CopyToInstance(UUserWidget* Widget)
{
}



#if WITH_EDITOR
bool FWidgetTemplate_QuestInventory::IsDataValid() const
{
	if (WidgetClass.IsNull() || SourceId.IsNone() || !AssetType.IsValid() || Items.IsEmpty())
	{
		return false;
	}
	return true;
}
void FWidgetTemplate_QuestInventory::AppendAssetBundleData(FAssetBundleData& AssetBundle, const FName& Bundle)
{
	AssetBundle.AddBundleAsset(Bundle, WidgetClass.ToSoftObjectPath().GetAssetPath());
}
#endif

TSubclassOf<UUserWidget> FWidgetTemplate_QuestInventory::GetWidgetClass() const
{
	return WidgetClass.Get();
}

void FWidgetTemplate_QuestInventory::CopyToInstance(UUserWidget* Widget)
{
	UQuestInteractInventoryUI* QuestUI = CastChecked<UQuestInteractInventoryUI>(Widget);
	QuestUI->InventorySourceId = SourceId;
	QuestUI->InventoryAssetType = AssetType;
	QuestUI->InventoryItems = Items;
}
