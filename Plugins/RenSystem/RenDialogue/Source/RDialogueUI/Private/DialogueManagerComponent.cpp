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
		Subsystem->OnDialogueAdded.BindUObject(this, &UDialogueManagerComponent::HandleOnDialogueAdded);
		Subsystem->OnDialogueRemoved.BindUObject(this, &UDialogueManagerComponent::HandleOnDialogueRemoved);
	}
}

void UDialogueManagerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UDialogueSubsystem* Subsystem = UDialogueSubsystem::Get(GetWorld());
	if (IsValid(Subsystem))
	{
		Subsystem->OnDialogueAdded.Unbind();
		Subsystem->OnDialogueRemoved.Unbind();
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

void UDialogueManagerComponent::HandleOnDialogueAdded(FPrimaryAssetId AssetId, UDialogueEngine* Engine)
{
	if (WidgetCollection.Contains(AssetId))
	{
		LOG_ERROR(LogEventflowEngine, TEXT("Widget already exists with id"));
		return;
	}

	APlayerController* PlayerController = GetPlayerController();
	if (!IsValid(PlayerController))
	{
		return;
	}

	const UDialogueAsset* Asset = Engine->GetAsset<UDialogueAsset>();
	if (!IsValid(Asset) || !IsValid(Asset->DialogueWidget))
	{
		return;
	}

	UDialogueUI* Widget = CreateWidget<UDialogueUI>(PlayerController, Asset->DialogueWidget);
	if (!IsValid(Widget))
	{
		LOG_ERROR(LogEventflowEngine, TEXT("Failed to create dialogue widget"));
		return;
	}

	WidgetCollection.Add(AssetId, Widget);
	Widget->InitializeDialogue(Asset, Engine);
	Widget->AddToViewport();
}

void UDialogueManagerComponent::HandleOnDialogueRemoved(FPrimaryAssetId AssetId, UDialogueEngine* Engine)
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

