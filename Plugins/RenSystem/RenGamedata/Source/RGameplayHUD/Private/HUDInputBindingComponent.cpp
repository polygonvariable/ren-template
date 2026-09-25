// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "HUDInputBindingComponent.h"

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
#include "HUDInputBindingAsset.h"
#include "Util/SubsystemUtil.h"


UHUDInputBindingComponent::UHUDInputBindingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	bAutoActivate = false;
}

void UHUDInputBindingComponent::BeginPlay()
{
	Super::BeginPlay();

	FStreamableManager& Manager = UAssetManager::GetStreamableManager();
	AssetHandle = Manager.RequestAsyncLoad(InputBinding.ToSoftObjectPath(), FStreamableDelegate::CreateUObject(this, &UHUDInputBindingComponent::HandleOnAssetLoaded));
}

void UHUDInputBindingComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GameplayMode = nullptr;

	UEnhancedInputComponent* InputComponent = GetInputComponent();
	if (IsValid(InputComponent))
	{
		for (const uint8& Handle : InputHandles)
		{
			InputComponent->RemoveBindingByHandle(Handle);
		}
		InputHandles.Empty();
	}

	FAssetManagerUtil::CancelHandle(AssetHandle);
	InputBindingAsset = nullptr;

	FAssetManagerUtil::CancelHandle(WidgetHandle);
	for (TPair<FGuid, TObjectPtr<UUserWidget>>& Kv : WidgetCollection)
	{
		if (IsValid(Kv.Value))
		{
			Kv.Value->RemoveFromParent();
		}
	}
	WidgetCollection.Empty();

	Super::EndPlay(EndPlayReason);
}

#if WITH_EDITOR
EDataValidationResult UHUDInputBindingComponent::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	if (InputBinding.IsNull())
	{
		Context.AddError(FText::FromString("Input binding asset is invalid"));
		return EDataValidationResult::Invalid;
	}

	return Result;
}
#endif

APlayerController* UHUDInputBindingComponent::GetPlayerController() const
{
	AHUD* HUD = Cast<AHUD>(GetOwner());
	if (!IsValid(HUD))
	{
		return nullptr;
	}
	return HUD->GetOwningPlayerController();
}

UEnhancedInputComponent* UHUDInputBindingComponent::GetInputComponent() const
{
	APlayerController* PlayerController = GetPlayerController();
	if (!IsValid(PlayerController))
	{
		return nullptr;
	}
	return Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
}

void UHUDInputBindingComponent::HandleOnAssetLoaded()
{
	FAssetManagerUtil::ReleaseHandle(AssetHandle);
	InputBindingAsset = InputBinding.Get();

	GameplayMode = FSubsystemLibrary::GetSubsystemInterface<IGameplayModeProvider>(GetWorld());

	UEnhancedInputComponent* InputComponent = GetInputComponent();
	if (IsValid(InputBindingAsset) && IsValid(InputComponent))
	{
		const TArray<FHUDInputBinding>& Bindings = InputBindingAsset->Bindings;
		for (const FHUDInputBinding& Binding : Bindings)
		{
			if (Binding.IsValid())
			{
				FGuid InputId = Binding.InputId;
				FSoftObjectPath Widget = Binding.WidgetClass.ToSoftObjectPath();

				FEnhancedInputActionEventBinding& EventBinding = InputComponent->BindAction(Binding.InputAction, ETriggerEvent::Triggered, this, &UHUDInputBindingComponent::HandleOnInputTriggered, InputId, Widget);
				InputHandles.Add(EventBinding.GetHandle());
			}
		}
	}
}

void UHUDInputBindingComponent::HandleOnInputTriggered(const FInputActionValue& Value, FGuid InputId, FSoftObjectPath Widget)
{
	FAssetManagerUtil::CancelHandle(WidgetHandle);

	TPair<FGuid, TObjectPtr<UUserWidget>>* FoundPair = WidgetCollection.FindByPredicate([InputId](const TPair<FGuid, TObjectPtr<UUserWidget>>& Kv) { return Kv.Key == InputId; });
	if (!FoundPair)
	{
		FStreamableManager& Manager = UAssetManager::GetStreamableManager();
		WidgetHandle = Manager.RequestAsyncLoad(Widget, FStreamableDelegate::CreateUObject(this, &UHUDInputBindingComponent::HandleOnWidgetLoaded, InputId, Widget));
		return;
	}

	UUserWidget* FoundWidget = FoundPair->Value.Get();
	if (IsValid(FoundWidget))
	{
		FoundWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void UHUDInputBindingComponent::HandleOnWidgetLoaded(FGuid InputId, FSoftObjectPath Widget)
{
	FAssetManagerUtil::ReleaseHandle(WidgetHandle);

	UObject* WidgetObject = Widget.ResolveObject();
	if (!IsValid(WidgetObject))
	{
		return;
	}

	UClass* WidgetClass = Cast<UClass>(WidgetObject);
	if (!IsValid(WidgetClass))
	{
		return;
	}

	AHUD* HUD = Cast<AHUD>(GetOwner());
	APlayerController* PlayerController = HUD->GetOwningPlayerController();

	UUserWidget* NewWidget = CreateWidget<UUserWidget>(PlayerController, WidgetClass);
	if (!IsValid(NewWidget))
	{
		return;
	}

	WidgetCollection.Add(TPair<FGuid, TObjectPtr<UUserWidget>>(InputId, NewWidget));
	NewWidget->OnVisibilityChanged.AddDynamic(this, &UHUDInputBindingComponent::HandleOnWidgetVisibilityChanged);
	NewWidget->AddToViewport();
	NewWidget->SetVisibility(ESlateVisibility::Visible);
}

void UHUDInputBindingComponent::HandleOnWidgetVisibilityChanged(ESlateVisibility Visiblity)
{
	if (!GameplayMode)
	{
		return;
	}

	if (Visiblity == ESlateVisibility::Visible)
	{
		FGameplayTagContainer R;
		R.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Gameplay.Input.HUD")));
		R.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Gameplay.Input.Character")));
		R.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Gameplay.Input.Camera")));
		R.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Gameplay.HUD.Visible")));
		GameplayMode->RempoveGameplayMode(R);

		FGameplayTagContainer A;
		A.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Gameplay.Mouse.Visible")));
		GameplayMode->AddGameplayMode(A);
	}
	else if (Visiblity == ESlateVisibility::Collapsed)
	{
		FGameplayTagContainer R;
		R.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Gameplay.Input.HUD")));
		R.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Gameplay.Input.Character")));
		R.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Gameplay.Input.Camera")));
		R.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Gameplay.HUD.Visible")));
		GameplayMode->AddGameplayMode(R);

		FGameplayTagContainer A;
		A.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Gameplay.Mouse.Visible")));
		GameplayMode->RempoveGameplayMode(A);
	}
}

