// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "GameplayHUD.h"

// Engine Headers
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif
#include "Blueprint/UserWidget.h"
#include "Engine/AssetManager.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/HUD.h"

// Project Headers
#include "Core/AssetManagerUtil.h"
#include "Core/GameplayModeProvider.h"
#include "Util/SubsystemUtil.h"


AGameplayHUD::AGameplayHUD()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void AGameplayHUD::BeginPlay()
{
	Super::BeginPlay();

	FStreamableManager& Manager = UAssetManager::GetStreamableManager();
	AssetHandle = Manager.RequestAsyncLoad(HUDWidgetClass.ToSoftObjectPath(), FStreamableDelegate::CreateUObject(this, &AGameplayHUD::HandleOnAssetLoaded));
}

void AGameplayHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	FAssetManagerUtil::CancelHandle(AssetHandle);

	if (GameplayMode)
	{
		GameplayMode->UnregisterTagNotify(HUDVisiblityTag, this);
	}
	GameplayMode = nullptr;

    if (IsValid(HUDWidget))
    {
        HUDWidget->RemoveFromParent();
    }
    HUDWidget = nullptr;

	Super::EndPlay(EndPlayReason);
}

#if WITH_EDITOR
EDataValidationResult AGameplayHUD::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	if (!HUDVisiblityTag.IsValid())
	{
		Context.AddError(FText::FromString("HUD visibility tag is invalid"));
		return EDataValidationResult::Invalid;
	}

	if (HUDWidgetClass.IsNull())
	{
		Context.AddError(FText::FromString("HUD widget class is invalid"));
		return EDataValidationResult::Invalid;
	}

	return Result;
}
#endif

void AGameplayHUD::HandleOnAssetLoaded()
{
	GameplayMode = FSubsystemLibrary::GetSubsystemInterface<IGameplayModeProvider>(GetWorld());
	if (GameplayMode)
	{
		HandleOnHUDVisiblityTagChanged(GameplayMode->HasTagExact(HUDVisiblityTag));
		GameplayMode->RegisterTagNotify(HUDVisiblityTag, FOnGameplayModeTagChanged::FDelegate::CreateUObject(this, &AGameplayHUD::HandleOnHUDVisiblityTagChanged));
	}
}

void AGameplayHUD::HandleOnHUDVisiblityTagChanged(bool bAdded)
{
	APlayerController* PlayerController = GetOwningPlayerController();
    if (!IsValid(PlayerController))
    {
        return;
    }

	if (bAdded)
	{
		if (!IsValid(HUDWidget))
		{
			UClass* WidgetClass = HUDWidgetClass.Get();
			if (!IsValid(WidgetClass))
			{
				return;
			}

			HUDWidget = CreateWidget<UUserWidget>(PlayerController, WidgetClass);
			if (!IsValid(HUDWidget))
			{
				return;
			}

			HUDWidget->AddToViewport();
		}

		HUDWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		if (IsValid(HUDWidget))
		{
			HUDWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

