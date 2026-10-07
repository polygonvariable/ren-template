// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "DialogueManagerComponent.h"

// Engine Headers
#include "Blueprint/UserWidget.h"
#include "GameFramework/HUD.h"

// Project Headers
#include "DialogueAsset.h"
#include "DialogueEngine.h"
#include "DialogueSubsystem.h"
#include "DialogueUI.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"


UDialogueManagerComponent::UDialogueManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	bAutoActivate = false;
}

void UDialogueManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	UDialogueSubsystem* Subsystem = UDialogueSubsystem::Get(GetWorld());
	if (IsValid(Subsystem))
	{
		Subsystem->GetOnEngineAdded().AddUObject(this, &UDialogueManagerComponent::HandleOnDialogueAdded);
		Subsystem->GetOnEngineRemoved().AddUObject(this, &UDialogueManagerComponent::HandleOnDialogueRemoved);
	}
}

void UDialogueManagerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UDialogueSubsystem* Subsystem = UDialogueSubsystem::Get(GetWorld());
	if (IsValid(Subsystem))
	{
		Subsystem->GetOnEngineAdded().RemoveAll(this);
		Subsystem->GetOnEngineRemoved().RemoveAll(this);
	}

	for (TPair<FPrimaryAssetId, TObjectPtr<UDialogueUI>>& Kv : WidgetCollection)
	{
		UDialogueUI* Widget = Kv.Value.Get();
		if (IsValid(Widget))
		{
			Widget->DeinitializeDialogue();
			Widget->RemoveFromParent();
		}
	}
	WidgetCollection.Empty();

	Super::EndPlay(EndPlayReason);
}

APlayerController* UDialogueManagerComponent::GetPlayerController() const
{
	AHUD* HUD = Cast<AHUD>(GetOwner());
	if (!IsValid(HUD))
	{
		return nullptr;
	}
	return HUD->GetOwningPlayerController();
}

void UDialogueManagerComponent::HandleOnDialogueAdded(FPrimaryAssetId AssetId, UEventflowEngine* Engine)
{
	UDialogueEngine* DialogueEngine = Cast<UDialogueEngine>(Engine);
	if (WidgetCollection.Contains(AssetId) || !IsValid(DialogueEngine))
	{
		LOG_ERROR(LogDialogue, TEXT("Widget already exists with id or dialogue engine is invalid"));
		return;
	}

	const UDialogueAsset* Asset = Engine->GetAsset<UDialogueAsset>();
	APlayerController* PlayerController = GetPlayerController();
	if (!IsValid(PlayerController) || !IsValid(Asset) || !IsValid(Asset->DialogueWidget))
	{
		LOG_ERROR(LogDialogue, TEXT("Player controller, asset or widget class is invalid"));
		return;
	}

	UDialogueUI* Widget = CreateWidget<UDialogueUI>(PlayerController, Asset->DialogueWidget);
	if (!IsValid(Widget))
	{
		LOG_ERROR(LogDialogue, TEXT("Failed to create dialogue widget"));
		return;
	}

	WidgetCollection.Add(AssetId, Widget);
	Widget->InitializeDialogue(Asset, DialogueEngine);
	Widget->SetFocus();
	Widget->AddToViewport();
}

void UDialogueManagerComponent::HandleOnDialogueRemoved(FPrimaryAssetId AssetId, UEventflowEngine* Engine)
{
	TObjectPtr<UDialogueUI>* FoundWidget = WidgetCollection.Find(AssetId);
	if (FoundWidget)
	{
		UDialogueUI* Widget = FoundWidget->Get();
		if (IsValid(Widget))
		{
			Widget->DeinitializeDialogue();
			Widget->RemoveFromParent();
		}
	}
	WidgetCollection.Remove(AssetId);
}

