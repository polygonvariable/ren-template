// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "GameplayModeDebug.h"

// Engine Headers
#include "Engine/AssetManager.h"
#include "EngineUtils.h"
#include "SlateIM.h"

// Project Headers
#include "MiscLibrary.h"
#include "GameplayModeSubsystem.h"


void FGameplayModeDebugWidget::EnableWidget()
{
	FSlateIMWidgetBase::EnableWidget();
}

void FGameplayModeDebugWidget::DisableWidget()
{
	FSlateIMWidgetBase::DisableWidget();
}

void FGameplayModeDebugWidget::DrawWidget(float DeltaTime)
{
	UGameplayModeSubsystem* Subsystem = GetSubsystem();

	SlateIM::FWindowParams WindowParams;
	WindowParams.WindowSize = FVector2f(150.0f * 2, 250.0f * 1.5);
	WindowParams.bAlwaysOnTop = true;

	if (SlateIM::BeginWindowRoot(TEXT("GameplayMode_Window"), WindowParams))
	{
		SlateIM::BeginTable();
		SlateIM::AddTableColumn(TEXT("GameplayMode_Debug"), TEXT("GameplayMode Debugger:"));
		{
			const TArray<FName>& ModeStack = Subsystem->GetEditorGameplayModeStack();
			if (SlateIM::NextTableCell())
			{
				SlateIM::BeginHorizontalStack();
				{
					SlateIM::Text(TEXT("Stack Size:"));
					SlateIM::Text(FString::FromInt(ModeStack.Num()), FColor::Cyan);
				}
				SlateIM::EndHorizontalStack();
			}

			if (SlateIM::NextTableCell())
			{
				int Num = ModeStack.Num();
				for (int i = 0; i < Num; i++)
				{
					SlateIM::BeginHorizontalStack();
					{
						SlateIM::Text(TEXT("[" + FString::FromInt(i) + "]"), FColor::Yellow);
						SlateIM::Text(ModeStack[i].ToString());
					}
					SlateIM::EndHorizontalStack();
				}
			}

			SlateIM::Text(FString::ChrN(40, TEXT('-')));

			const FGameplayTagContainer& ActiveTags = Subsystem->GetGameplayModeTags();
			if (SlateIM::NextTableCell())
			{
				SlateIM::BeginHorizontalStack();
				{
					SlateIM::Text(TEXT("Active Tags:"));
					SlateIM::Text(FString::FromInt(ActiveTags.Num()), FColor::Cyan);
				}
				SlateIM::EndHorizontalStack();

				int Num = ActiveTags.Num();
				for (int i = 0; i < Num; i++)
				{
					SlateIM::BeginHorizontalStack();
					{
						SlateIM::Text(TEXT("[" + FString::FromInt(i) + "]"), FColor::Yellow);
						SlateIM::Text(ActiveTags.GetByIndex(i).ToString());
					}
					SlateIM::EndHorizontalStack();
				}
			}
		}
		SlateIM::EndTable();
	}
	SlateIM::EndRoot();
}

UGameplayModeSubsystem* FGameplayModeDebugWidget::GetSubsystem()
{
	if (GameplayModeSubsystem.IsValid())
	{
		return GameplayModeSubsystem.Get();
	}

	GameplayModeSubsystem = UGameplayModeSubsystem::Get(FMiscLibrary::GetCurrentWorld());
	return GameplayModeSubsystem.Get();
}

