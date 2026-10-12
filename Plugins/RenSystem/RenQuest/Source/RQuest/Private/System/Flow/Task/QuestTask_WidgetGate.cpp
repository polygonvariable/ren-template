// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/Flow/Task/QuestTask_WidgetGate.h"

// Engine Headers
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif
#include "Blueprint/UserWidget.h"

// Project Headers
#if WITH_EDITOR
#include "Core/QuestSettings.h"
#endif
#include "Core/IObjectiveFeedback.h"
#include "GameplayModeProvider.h"
#include "SubsystemLibrary.h"


void UQuestTask_WidgetGate::CopyFromAsset(const UEventflowTask* Template)
{
	Super::CopyFromAsset(Template);

	const UQuestTask_WidgetGate* Task = Cast<UQuestTask_WidgetGate>(Template);
	checkf(IsValid(Template), TEXT("Node task contains invalid template data"));

	WidgetGameplayMode = Task->WidgetGameplayMode;
}

#if WITH_EDITOR
void UQuestTask_WidgetGate::AppendAssetBundleData(FAssetBundleData& AssetBundle)
{
	Super::AppendAssetBundleData(AssetBundle);

	const UQuestSettings* Settings = UQuestSettings::Get();
	const FName& BundleName = Settings->BundleName;

	FWidgetTemplateDefinition* WidgetDefinition = WidgetTemplate.GetMutablePtr();
	if (WidgetDefinition)
	{
		WidgetDefinition->AppendAssetBundleData(AssetBundle, BundleName);
	}
}
EDataValidationResult UQuestTask_WidgetGate::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	if (WidgetGameplayMode.IsNone())
	{
		Context.AddError(FText::FromString("Widget gameplay mode is empty"));
		return EDataValidationResult::Invalid;
	}

	const FWidgetTemplateDefinition* RawWidget = WidgetTemplate.GetPtr();
	if (!RawWidget || !RawWidget->IsDataValid())
	{
		Context.AddError(FText::FromString("Widget template is invalid or contains invalid data"));
		return EDataValidationResult::Invalid;
	}

	return Result;
}
#endif

void UQuestTask_WidgetGate::HandleOnObjectiveFeedback(EFSMResult Result)
{
	int Index = 2;
	switch (Result)
	{
	case EFSMResult::Success:
		Index = 0;
		break;
	case EFSMResult::Failed:
		Index = 1;
		break;
	case EFSMResult::Cancelled:
		Index = 2;
		break;
	}

	ModifyTransitionData(
		[Index](TInstancedStruct<FEventflowNodeTransitionData>& TransitionData)
		{
			FEventflowNodeTransitionData& Data = TransitionData.GetMutable();
			Data.NextNodeIndex = Index;
		}
	);

	Finish(Result);
}

void UQuestTask_WidgetGate::OnInitialized(EFSMState PreviousState)
{
	Super::OnInitialized(PreviousState);
	Load();
}

void UQuestTask_WidgetGate::OnLoaded(EFSMState PreviousState)
{
	UQuestTask_WidgetGate* Template = GetOwningTemplate<UQuestTask_WidgetGate>();
	checkf(IsValid(Template), TEXT("Node task contains invalid template data"));

	FWidgetTemplateDefinition* WidgetData = Template->WidgetTemplate.GetMutablePtr();
	checkf(WidgetData, TEXT("Widget template data is invalid"));

	Widget = WidgetData->CreateWidget(GetWorld());
	checkf(IsValid(Widget), TEXT("Failed to create widget"));

	IObjectiveFeedback* WidgetObjective = CastChecked<IObjectiveFeedback>(Widget);
	WidgetObjective->GetOnObjectiveFeedback().BindUObject(this, &UQuestTask_WidgetGate::HandleOnObjectiveFeedback);

	Ready();
}

void UQuestTask_WidgetGate::OnReady(EFSMState PreviousState)
{
	Active();
}

void UQuestTask_WidgetGate::OnActive(EFSMState PreviousState)
{
	IGameplayModeProvider* GameplayMode = FSubsystemLibrary::GetSubsystemInterface<IGameplayModeProvider>(GetWorld());
	if (GameplayMode)
	{
		GameplayMode->PushGameplayMode(WidgetGameplayMode);
	}

	Widget->AddToViewport();
	Widget->SetFocus();
}

void UQuestTask_WidgetGate::OnReset()
{
	IGameplayModeProvider* GameplayMode = FSubsystemLibrary::GetSubsystemInterface<IGameplayModeProvider>(GetWorld());
	if (GameplayMode)
	{
		GameplayMode->PopGameplayMode(WidgetGameplayMode);
	}

	if (IsValid(Widget))
	{
		IObjectiveFeedback* WidgetObjective = CastChecked<IObjectiveFeedback>(Widget);
		WidgetObjective->GetOnObjectiveFeedback().Unbind();

		Widget->RemoveFromParent();
	}
	Widget = nullptr;

	Super::OnReset();
}
