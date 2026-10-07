// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "DialogueDebug.h"

// Engine Headers
#include "Engine/AssetManager.h"
#include "EngineUtils.h"
#include "SlateIM.h"

// Project Headers
#include "MiscLibrary.h"
#include "DialogueEngine.h"
#include "DialogueSubsystem.h"
#include "EventflowTask.h"
#include "Task/EventflowSubTask.h"
#include "Task/EventflowNodeTask.h"
#include "DialogueTask.h"


void FDialogueDebugWidget::EnableWidget()
{
	FSlateIMWidgetBase::EnableWidget();
}

void FDialogueDebugWidget::DisableWidget()
{
	FSlateIMWidgetBase::DisableWidget();
}

void FDialogueDebugWidget::DrawWidget(float DeltaTime)
{
	UDialogueSubsystem* Subsystem = GetSubsystem();

	SlateIM::FWindowParams WindowParams;
	WindowParams.WindowSize = FVector2f(150.0f * 4.5, 250.0f * 1.5);
	WindowParams.bAlwaysOnTop = true;

	if (SlateIM::BeginWindowRoot(TEXT("Dialogue_Window"), WindowParams))
	{
		SlateIM::BeginScrollBox();
		if (IsValid(Subsystem))
		{
			SlateIM::BeginTable();
			SlateIM::AddTableColumn(TEXT("Dialogue_Debug"), TEXT("Dialogue Debugger:"));
			{
				const TMap<FPrimaryAssetId, TObjectPtr<UDialogueEngine>>& Dialogues = Subsystem->GetEditorDialogues();

				if (SlateIM::NextTableCell())
				{
					SlateIM::BeginHorizontalStack();
					{
						SlateIM::Text(TEXT("Pool Size:"));
						SlateIM::Text(FString::FromInt(Subsystem->GetEditorDialoguePoolSize()), FColor::Cyan);
					}
					SlateIM::EndHorizontalStack();
				}

				int DialogueIndex = 0;
				for (const TPair<FPrimaryAssetId, TObjectPtr<UDialogueEngine>>& Kv : Dialogues)
				{
					if (SlateIM::NextTableCell())
					{
						UDialogueEngine* Engine = Kv.Value;
						
						SlateIM::Text(TEXT("----------------------------------------------------"));
						if (!IsValid(Engine))
						{
							SlateIM::Text(TEXT("Dialogue:"));
							SlateIM::Text(TEXT("(Invalid Dialogue)"), FColor::Red);
							continue;
						}

						SlateIM::BeginHorizontalStack();
						{
							SlateIM::Text(TEXT("Dialogue:"));
							SlateIM::Text(FString::FromInt(DialogueIndex), FColor::Cyan);
						}
						SlateIM::EndHorizontalStack();

						SlateIM::BeginTable();
						{
							SlateIM::BeginTableHeader();
							{
								SlateIM::InitialTableColumnWidth(150.0f);
								SlateIM::AddTableColumn(TEXT("Dialogue_AssetId"), TEXT("AssetId"));
								SlateIM::InitialTableColumnWidth(150.0f);
								SlateIM::AddTableColumn(TEXT("Dialogue_Title"), TEXT("Title"));
								SlateIM::InitialTableColumnWidth(150.0f);
								SlateIM::AddTableColumn(TEXT("Dialogue_EngineState"), TEXT("State"));
								SlateIM::InitialTableColumnWidth(150.0f);
								SlateIM::AddTableColumn(TEXT("Dialogue_EngineResult"), TEXT("Result"));
							}
							SlateIM::EndTableHeader();

							SlateIM::BeginTableBody();
							{
								SlateIM::NextTableCell();
								SlateIM::Text(Kv.Key.ToString());

								FString TitleString = Engine->GetFName().ToString();
								FString StateString = StaticEnum<EFSMState>()->GetNameStringByValue(static_cast<int64>(Engine->GetState()));
								FString ResultString = StaticEnum<EFSMResult>()->GetNameStringByValue(static_cast<int64>(Engine->GetResult()));

								SlateIM::NextTableCell();
								SlateIM::Text(TitleString);

								SlateIM::NextTableCell();
								SlateIM::Text(StateString);

								SlateIM::NextTableCell();
								SlateIM::Text(ResultString);
							}
							SlateIM::EndTableBody();
						}
						SlateIM::EndTable();
						

						UEventflowNodeTask* NodeTask = Engine->GetTask();
						if (!IsValid(NodeTask))
						{
							SlateIM::BeginHorizontalStack();
							{
								SlateIM::Text(TEXT("Dialogue Task:"));
								SlateIM::Text(TEXT("(Invalid Task)"), FColor::Red);
							}
							SlateIM::EndHorizontalStack();
							continue;
						}

						SlateIM::BeginHorizontalStack();
						{
							SlateIM::Text(TEXT("Dialogue Task:"));
							SlateIM::Text(NodeTask->GetFName().ToString(), FColor::Cyan);
						}
						SlateIM::EndHorizontalStack();

						SlateIM::BeginTable();
						{
							SlateIM::BeginTableHeader();
							{
								SlateIM::InitialTableColumnWidth(150.0f);
								SlateIM::AddTableColumn(TEXT("State_State"), TEXT("State"));
								SlateIM::InitialTableColumnWidth(150.0f);
								SlateIM::AddTableColumn(TEXT("State_Result"), TEXT("Result"));
							}
							SlateIM::EndTableHeader();

							SlateIM::BeginTableBody();
							{
								FString StateString = StaticEnum<EFSMState>()->GetNameStringByValue(static_cast<int64>(NodeTask->GetState()));
								FString ResultString = StaticEnum<EFSMResult>()->GetNameStringByValue(static_cast<int64>(NodeTask->GetResult()));

								SlateIM::NextTableCell();
								SlateIM::Text(StateString);

								SlateIM::NextTableCell();
								SlateIM::Text(ResultString);
							}
							SlateIM::EndTableBody();
						}
						SlateIM::EndTable();


						int SubTaskIndex = 0;
						const TArray<TObjectPtr<UEventflowSubTask>>& SubTasks = NodeTask->GetSubTasks();
						for (TObjectPtr<UEventflowSubTask> SubTask : SubTasks)
						{
							if (!IsValid(SubTask))
							{
								SlateIM::BeginHorizontalStack();
								{
									SlateIM::Text(TEXT("Dialogue Sub Task:"));
									SlateIM::Text(TEXT("(Invalid Sub Task)"), FColor::Red);
								}
								SlateIM::EndHorizontalStack();
								continue;
							}

							SlateIM::BeginHorizontalStack();
							{
								SlateIM::Text(TEXT("Dialogue Sub Task:"), FColor::Cyan);
								SlateIM::Text(FString::FromInt(SubTaskIndex), FColor::Cyan);
							}
							SlateIM::EndHorizontalStack();

							SlateIM::BeginTable();
							{
								SlateIM::BeginTableHeader();
								{
									SlateIM::InitialTableColumnWidth(150.0f);
									SlateIM::AddTableColumn(TEXT("State_Name"), TEXT("Title"));
									SlateIM::InitialTableColumnWidth(150.0f);
									SlateIM::AddTableColumn(TEXT("State_State"), TEXT("State"));
									SlateIM::InitialTableColumnWidth(150.0f);
									SlateIM::AddTableColumn(TEXT("State_Result"), TEXT("Result"));
								}
								SlateIM::EndTableHeader();

								SlateIM::BeginTableBody();
								{
									FString TitleString = SubTask->GetFName().ToString();
									FString StateString = StaticEnum<EFSMState>()->GetNameStringByValue(static_cast<int64>(SubTask->GetState()));
									FString ResultString = StaticEnum<EFSMResult>()->GetNameStringByValue(static_cast<int64>(SubTask->GetResult()));

									SlateIM::NextTableCell();
									SlateIM::Text(TitleString);

									SlateIM::NextTableCell();
									SlateIM::Text(StateString);

									SlateIM::NextTableCell();
									SlateIM::Text(ResultString);
								}
								SlateIM::EndTableBody();
							}
							SlateIM::EndTable();

							SubTaskIndex++;
						}

					}
					DialogueIndex++;
				}





			}
			SlateIM::EndTable();
		}
		else
		{
			SlateIM::BeginHorizontalStack();
			{
				SlateIM::Text(TEXT("Dialogue Subsystem:"));
				SlateIM::Text(TEXT("Invalid"), FColor::Red);
			}
			SlateIM::EndHorizontalStack();
		}
		SlateIM::EndScrollBox();
	}
	SlateIM::EndRoot();
}

UDialogueSubsystem* FDialogueDebugWidget::GetSubsystem()
{
	if (DialogueSubsystem.IsValid())
	{
		return DialogueSubsystem.Get();
	}

	DialogueSubsystem = UDialogueSubsystem::Get(FMiscLibrary::GetCurrentWorld());
	return DialogueSubsystem.Get();
}

