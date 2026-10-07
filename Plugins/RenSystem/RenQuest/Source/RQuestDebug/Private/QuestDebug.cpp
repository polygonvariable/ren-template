// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "QuestDebug.h"

// Engine Headers
#include "Engine/AssetManager.h"
#include "EngineUtils.h"
#include "SlateIM.h"

// Project Headers
#include "MiscLibrary.h"
#include "System/Flow/QuestEngine.h"
#include "System/QuestSubsystem.h"
#include "EventflowTask.h"
#include "Task/EventflowSubTask.h"
#include "Task/EventflowNodeTask.h"
#include "System/Flow/Task/QuestPrimaryTask.h"
#include "System/Flow/Task/QuestSubTask.h"


void FQuestDebugWidget::EnableWidget()
{
	FSlateIMWidgetBase::EnableWidget();
}

void FQuestDebugWidget::DisableWidget()
{
	FSlateIMWidgetBase::DisableWidget();
}

void FQuestDebugWidget::DrawWidget(float DeltaTime)
{
	UQuestSubsystem* Subsystem = GetSubsystem();

	SlateIM::FWindowParams WindowParams;
	WindowParams.WindowSize = FVector2f(150.0f * 3, 250.0f * 1.5);
	WindowParams.bAlwaysOnTop = true;

	if (SlateIM::BeginWindowRoot(TEXT("Quest_Window"), WindowParams))
	{
		SlateIM::BeginScrollBox();
		if (IsValid(Subsystem))
		{
			SlateIM::BeginTable();
			SlateIM::AddTableColumn(TEXT("Quest_Debug"), TEXT("Quest Debugger:"));
			{
				const TMap<FPrimaryAssetId, TObjectPtr<UQuestEngine>>& Quests = Subsystem->GetEditorQuests();

				int QuestIndex = 0;
				for (const TPair<FPrimaryAssetId, TObjectPtr<UQuestEngine>>& Kv : Quests)
				{
					if (SlateIM::NextTableCell())
					{
						UQuestEngine* Engine = Kv.Value;
						
						SlateIM::Text(TEXT("----------------------------------------------------"));
						if (!IsValid(Engine))
						{
							SlateIM::Text("[" + FString::FromInt(QuestIndex) + "]", FColor::Yellow);
							SlateIM::Text(TEXT("Engine:"));
							SlateIM::Text(TEXT("(Invalid Engine)"), FColor::Red);
							continue;
						}

						SlateIM::BeginHorizontalStack();
						{
							SlateIM::Text("[" + FString::FromInt(QuestIndex) + "]", FColor::Yellow);
							SlateIM::Text(TEXT("Engine:"));
							SlateIM::Text(Engine->GetFName().ToString(), FColor::Magenta);
						}
						SlateIM::EndHorizontalStack();

						SlateIM::BeginTable();
						{
							SlateIM::BeginTableHeader();
							{
								SlateIM::InitialTableColumnWidth(150.0f);
								SlateIM::AddTableColumn(TEXT("Quest_AssetId"), TEXT("AssetId"));
								SlateIM::InitialTableColumnWidth(150.0f);
								SlateIM::AddTableColumn(TEXT("Quest_EngineState"), TEXT("State"));
								SlateIM::InitialTableColumnWidth(150.0f);
								SlateIM::AddTableColumn(TEXT("Quest_EngineResult"), TEXT("Result"));
							}
							SlateIM::EndTableHeader();

							SlateIM::BeginTableBody();
							{
								SlateIM::NextTableCell();
								SlateIM::Text(Kv.Key.ToString());

								FString StateString = StaticEnum<EFSMState>()->GetNameStringByValue(static_cast<int64>(Engine->GetState()));
								FString ResultString = StaticEnum<EFSMResult>()->GetNameStringByValue(static_cast<int64>(Engine->GetResult()));

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
								SlateIM::Text(TEXT("[>]"), FColor::Green);
								SlateIM::Text(TEXT("Task:"));
								SlateIM::Text(TEXT("(Invalid Task)"), FColor::Red);
							}
							SlateIM::EndHorizontalStack();
							continue;
						}

						SlateIM::BeginHorizontalStack();
						{
							SlateIM::Text(TEXT("[>]"), FColor::Green);
							SlateIM::Text(TEXT("Task:"));
							SlateIM::Text(NodeTask->GetFName().ToString(), FColor::Magenta);
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
									SlateIM::Text(TEXT("[" + FString::FromInt(SubTaskIndex) + "]"), FColor::Magenta);
									SlateIM::Text(TEXT("Sub Task:"));
									SlateIM::Text(TEXT("(Invalid Sub Task)"), FColor::Red);
								}
								SlateIM::EndHorizontalStack();
								continue;
							}

							SlateIM::BeginHorizontalStack();
							{
								SlateIM::Text(TEXT("[" + FString::FromInt(SubTaskIndex) + "]"), FColor::Magenta);
								SlateIM::Text(TEXT("Sub Task:"));
								SlateIM::Text(SubTask->GetFName().ToString(), FColor::Magenta);
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
									FString StateString = StaticEnum<EFSMState>()->GetNameStringByValue(static_cast<int64>(SubTask->GetState()));
									FString ResultString = StaticEnum<EFSMResult>()->GetNameStringByValue(static_cast<int64>(SubTask->GetResult()));

									SlateIM::NextTableCell();
									SlateIM::Text(StateString);

									SlateIM::NextTableCell();
									SlateIM::Text(ResultString);
								}
								SlateIM::EndTableBody();
							}
							SlateIM::EndTable();

							TArray<FString> SubTaskDebug;
							SubTask->GetEditorDebugInfo(SubTaskDebug);
							for (const FString& Debug : SubTaskDebug)
							{
								SlateIM::Text(Debug, FColor::Blue);
							}

							SubTaskIndex++;
						}

					}
					QuestIndex++;
				}
			}
			SlateIM::EndTable();
		}
		else
		{
			SlateIM::BeginHorizontalStack();
			{
				SlateIM::Text(TEXT("Quest Subsystem:"));
				SlateIM::Text(TEXT("Invalid"), FColor::Red);
			}
			SlateIM::EndHorizontalStack();
		}
		SlateIM::EndScrollBox();
	}
	SlateIM::EndRoot();
}

UQuestSubsystem* FQuestDebugWidget::GetSubsystem()
{
	if (QuestSubsystem.IsValid())
	{
		return QuestSubsystem.Get();
	}

	QuestSubsystem = UQuestSubsystem::Get(FMiscLibrary::GetCurrentWorld());
	return QuestSubsystem.Get();
}

