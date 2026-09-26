// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "GameplayInputManagerComponent.h"

// Engine Headers
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif
#include "Engine/AssetManager.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

// Project Headers
#include "Core/AssetManagerUtil.h"
#include "Core/GameplayModeProvider.h"
#include "Util/SubsystemUtil.h"


UGameplayInputManagerComponent::UGameplayInputManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	bAutoActivate = false;
}

void UGameplayInputManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	FStreamableManager& Manager = UAssetManager::GetStreamableManager();
	AssetHandle = Manager.RequestAsyncLoad(InputCollection.ToSoftObjectPath(), FStreamableDelegate::CreateUObject(this, &UGameplayInputManagerComponent::HandleOnInputAssetLoaded));
}

void UGameplayInputManagerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	InputAsset = nullptr;
	FAssetManagerUtil::CancelHandle(AssetHandle);

	if (GameplayMode)
	{
		GameplayMode->GetOnGameplayModeTagsChanged().RemoveAll(this);
	}
	GameplayMode = nullptr;

	Super::EndPlay(EndPlayReason);
}

#if WITH_EDITOR
EDataValidationResult UGameplayInputManagerComponent::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	if (InputCollection.IsNull())
	{
		Context.AddError(FText::FromString("Input asset is invalid"));
		return EDataValidationResult::Invalid;
	}

	return Result;
}
#endif

void UGameplayInputManagerComponent::HandleOnInputAssetLoaded()
{
	InputAsset = InputCollection.Get();

	GameplayMode = FSubsystemLibrary::GetSubsystemInterface<IGameplayModeProvider>(GetWorld());
	if (!GameplayMode)
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = GetInputSubsystem();
	if (IsValid(InputAsset) && IsValid(InputSubsystem))
	{
		const FGameplayTagContainer& CurrentTags = GameplayMode->GetGameplayModeTags();
		const TArray<FGameplayInputDefinition>& Inputs = InputAsset->Inputs;

		for (const FGameplayInputDefinition& Definition : Inputs)
		{
			if (Definition.GameplayModeTag.MatchesAnyExact(CurrentTags))
			{
				RegisterInputs(Definition, true, InputSubsystem);
			}
		}
	}

	GameplayMode->GetOnGameplayModeTagsChanged().AddUObject(this, &UGameplayInputManagerComponent::HandleOnGameplayModeTagsChanged);
}

void UGameplayInputManagerComponent::HandleOnGameplayModeTagsChanged(FGameplayTag Tag, bool bAdded)
{
	if (!IsValid(InputAsset))
	{
		return;
	}

	const FGameplayInputDefinition* Definition = InputAsset->Inputs.FindByPredicate([Tag](const FGameplayInputDefinition& Definition) { return Definition.GameplayModeTag.MatchesTag(Tag); });
	if (!Definition)
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = GetInputSubsystem();
	if (!IsValid(InputSubsystem))
	{
		return;
	}

	RegisterInputs(*Definition, bAdded, InputSubsystem);
}

UEnhancedInputLocalPlayerSubsystem* UGameplayInputManagerComponent::GetInputSubsystem() const
{
	APlayerController* Controller = Cast<APlayerController>(GetOuter());
	if (!IsValid(Controller))
	{
		return nullptr;
	}

	ULocalPlayer* LocalPlayer = Controller->GetLocalPlayer();
	if (!IsValid(LocalPlayer))
	{
		return nullptr;
	}
	return LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
}

void UGameplayInputManagerComponent::RegisterInputs(const FGameplayInputDefinition& Definition, bool bAdded, UEnhancedInputLocalPlayerSubsystem* InputSubsystem)
{
	if (bAdded)
	{
		for (UInputMappingContext* IMC : Definition.InputMappings)
		{
			if (!InputSubsystem->HasMappingContext(IMC))
			{
				InputSubsystem->AddMappingContext(IMC, 10);
			}
		}
	}
	else
	{
		for (UInputMappingContext* IMC : Definition.InputMappings)
		{
			if (InputSubsystem->HasMappingContext(IMC))
			{
				InputSubsystem->RemoveMappingContext(IMC);
			}
		}
	}
}

