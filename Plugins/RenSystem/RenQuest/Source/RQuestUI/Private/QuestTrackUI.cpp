// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "QuestTrackUI.h"

// Engine Headers
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif
#include "EnhancedInputComponent.h"

// Project Headers
#include "QuestRuntimeDataUI.h"
#include "System/Flow/QuestEngine.h"
#include "System/QuestSubsystem.h"


UQuestTrackUI::UQuestTrackUI(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SetIsFocusable(false);
}

#if WITH_EDITOR
EDataValidationResult UQuestTrackUI::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);
	if (!IsValid(CancelInput))
	{
		Context.AddWarning(FText::FromString("Invalid cancel input action, input wont be handled"));
	}
	return Result;
}
#endif

void UQuestTrackUI::HandleOnQuestDataChanged()
{
	if (IsValid(QuestSubsystem))
	{
		QuestData->ShowDetail(QuestSubsystem->GetFocusedQuest());
	}
}

void UQuestTrackUI::HandleOnQuestCancelInput(const FInputActionValue& Value)
{
	QuestData->CancelObjective();
}

void UQuestTrackUI::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerController* PlayerController = GetOwningPlayer();
	if (IsValid(PlayerController) && IsValid(CancelInput))
	{
		UEnhancedInputComponent* ControllerInput = Cast<UEnhancedInputComponent>((PlayerController->InputComponent));
		if (IsValid(ControllerInput))
		{
			FEnhancedInputActionEventBinding& Binding = ControllerInput->BindAction(CancelInput, ETriggerEvent::Triggered, this, &UQuestTrackUI::HandleOnQuestCancelInput);
			InputHandles.Add(Binding.GetHandle());
		}
	}

	QuestSubsystem = UQuestSubsystem::Get(GetWorld());
	if (IsValid(QuestSubsystem))
	{
		HandleOnQuestDataChanged();
		QuestSubsystem->OnQuestFocusChanged.AddWeakLambda(this, [this]() { HandleOnQuestDataChanged(); });
		QuestSubsystem->OnQuestDataChanged.AddWeakLambda(this, [this]() { HandleOnQuestDataChanged(); });
		QuestSubsystem->GetOnEngineAdded().AddWeakLambda(this, [this](FPrimaryAssetId, UEventflowEngine*) { HandleOnQuestDataChanged(); });
		QuestSubsystem->GetOnEngineRemoved().AddWeakLambda(this, [this](FPrimaryAssetId, UEventflowEngine*) { HandleOnQuestDataChanged(); });
	}
}

void UQuestTrackUI::NativeDestruct()
{
	APlayerController* PlayerController = GetOwningPlayer();
	if (IsValid(PlayerController))
	{
		UEnhancedInputComponent* ControllerInput = Cast<UEnhancedInputComponent>((PlayerController->InputComponent));
		if (IsValid(ControllerInput))
		{
			for (const uint8& Handle : InputHandles)
			{
				ControllerInput->RemoveBindingByHandle(Handle);
			}
			InputHandles.Empty();
		}
	}

	if (IsValid(QuestSubsystem))
	{
		QuestSubsystem->OnQuestFocusChanged.RemoveAll(this);
		QuestSubsystem->OnQuestDataChanged.RemoveAll(this);
		QuestSubsystem->GetOnEngineAdded().RemoveAll(this);
		QuestSubsystem->GetOnEngineRemoved().RemoveAll(this);
	}
	QuestSubsystem = nullptr;

	Super::NativeDestruct();
}

