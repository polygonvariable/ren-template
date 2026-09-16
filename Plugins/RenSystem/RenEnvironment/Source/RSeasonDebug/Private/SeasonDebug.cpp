// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "SeasonDebug.h"

// Engine Headers
#include "Engine/AssetManager.h"
#include "EngineUtils.h"
#include "SlateIM.h"

// Project Headers
#include "Data/SeasonAsset.h"
#include "Data/SeasonCollectionAsset.h"
#include "MiscLibrary.h"
#include "System/SeasonController.h"
#include "System/SeasonSubsystem.h"


void FSeasonDebugWidget::EnableWidget()
{
	FSlateIMWidgetBase::EnableWidget();
}

void FSeasonDebugWidget::DisableWidget()
{
	SeasonController.Reset();
	FSlateIMWidgetBase::DisableWidget();
}

void FSeasonDebugWidget::DrawWidget(float DeltaTime)
{
	USeasonController* Controller = GetController();

	SlateIM::FWindowParams WindowParams;
	WindowParams.WindowSize = FVector2f(150.0f * 2, 250.0f * 1.5);
	WindowParams.bAlwaysOnTop = true;

	if (SlateIM::BeginWindowRoot(TEXT("SeasonWindow"), WindowParams))
	{
		SlateIM::BeginTable();
		SlateIM::AddTableColumn(TEXT("Season_Debug"), TEXT("Season Debugger:"));

		if (Controller)
		{

			if (SlateIM::NextTableCell())
			{
				const TMap<int, TWeakObjectPtr<UObject>>& CollectionList = Controller->GetEditorSeasonCollectionList();

				SlateIM::HAlign(HAlign_Fill);

				SlateIM::BeginHorizontalStack();
				{
					SlateIM::Text(TEXT("Season Controller:"));
					SlateIM::Text(Controller->GetFName().ToString(), FColor::Cyan);
				}
				SlateIM::EndHorizontalStack();
				SlateIM::BeginHorizontalStack();
				{
					SlateIM::Text(TEXT("Season Collection List:"));
					SlateIM::Text(FString::FromInt(CollectionList.Num()), FColor::Cyan);
				}
				SlateIM::EndHorizontalStack();

				SlateIM::BeginTable();
				{
					SlateIM::BeginTableHeader();
					{
						SlateIM::InitialTableColumnWidth(150.0f);
						SlateIM::AddTableColumn(TEXT("Season_Priority"), TEXT("Priority"));
						SlateIM::InitialTableColumnWidth(150.0f);
						SlateIM::AddTableColumn(TEXT("Season_Asset"), TEXT("Collection"));
					}
					SlateIM::EndTableHeader();

					SlateIM::BeginTableBody();
					{
						for (const TPair<int, TWeakObjectPtr<UObject>>& Kv : CollectionList)
						{
							SlateIM::NextTableCell();
							SlateIM::Text(FString::FromInt(Kv.Key));

							UObject* CollectionAsset = Kv.Value.Get();

							SlateIM::NextTableCell();
							SlateIM::Text(IsValid(CollectionAsset) ? CollectionAsset->GetFName().ToString() : TEXT("(Invalid Asset)"));
						}
					}
					SlateIM::EndTableBody();
				}
				SlateIM::EndTable();
			}


			if (SlateIM::NextTableCell())
			{
				const USeasonCollectionAsset* CollectionAsset = Controller->GetEditorCurrentSeasonCollection();

				SlateIM::HAlign(HAlign_Fill);
				if (CollectionAsset)
				{
					SlateIM::BeginHorizontalStack();
					{
						SlateIM::Text(TEXT("Current Collection:"));
						SlateIM::Text(CollectionAsset->GetFName().ToString(), FColor::Cyan);
					}
					SlateIM::EndHorizontalStack();

					SlateIM::BeginTable();
					{
						SlateIM::BeginTableHeader();
						{
							SlateIM::InitialTableColumnWidth(150.0f);
							SlateIM::AddTableColumn(TEXT("Season_Asset"), TEXT("Season"));
							SlateIM::InitialTableColumnWidth(150.0f);
							SlateIM::AddTableColumn(TEXT("Season_Duration"), TEXT("Duration"));
						}
						SlateIM::EndTableHeader();

						SlateIM::BeginTableBody();
						{
							for (const FSeasonData& Item : CollectionAsset->Seasons)
							{
								SlateIM::NextTableCell();
								SlateIM::Text(IsValid(Item.Asset) ? Item.Asset->GetFName().ToString() : TEXT("(Invalid Asset)"));

								SlateIM::NextTableCell();
								SlateIM::Text(FString::FromInt(Item.Duration));
							}
						}
						SlateIM::EndTableBody();
					}
					SlateIM::EndTable();

					float Transition = 0.0f;
					const USeasonAsset* SeasonAsset = CollectionAsset->GetSeasonByDay(Controller->GetEditorCurrentDay(), Controller->GetEditorYearLength(), Transition);

					SlateIM::BeginHorizontalStack();
					{
						SlateIM::Text(TEXT("Current Season:"));
						SlateIM::Text(IsValid(SeasonAsset) ? SeasonAsset->GetFName().ToString() : TEXT("Invalid Asset"), FSlateColor(FColor::Cyan));
					}
					SlateIM::EndHorizontalStack();
					SlateIM::BeginHorizontalStack();
					{
						SlateIM::Text(TEXT("Current Transition:"));
						SlateIM::Text(FString::Printf(TEXT("%f"), Transition), FColor::Cyan);
					}
					SlateIM::EndHorizontalStack();
				}
				else
				{
					SlateIM::BeginHorizontalStack();
					{
						SlateIM::Text(TEXT("Current Collection:"));
						SlateIM::Text(TEXT("Invalid"), FColor::Red);
					}
					SlateIM::EndHorizontalStack();
				}
			}


			if (SlateIM::NextTableCell())
			{
				SlateIM::BeginHorizontalStack();
				{
					SlateIM::Text(TEXT("Year Length:"));
					SlateIM::Text(FString::FromInt(Controller->GetEditorYearLength()), FColor::Cyan);
				}
				SlateIM::EndHorizontalStack();
			}


			if (SlateIM::NextTableCell())
			{
				SlateIM::BeginHorizontalStack();
				{
					SlateIM::Text(TEXT("Day:"));
					SlateIM::Text(FString::FromInt(Controller->GetEditorCurrentDay()), FColor::Cyan);
				}
				SlateIM::EndHorizontalStack();
			}

		}
		else
		{
			if (SlateIM::NextTableCell())
			{
				SlateIM::BeginHorizontalStack();
				{
					SlateIM::Text(TEXT("Season Controller:"));
					SlateIM::Text(TEXT("Invalid"), FColor::Red);
				}
				SlateIM::EndHorizontalStack();
			}
		}
			
		SlateIM::EndTable();
	}
	SlateIM::EndRoot();
}

USeasonController* FSeasonDebugWidget::GetController()
{
	if (SeasonController.IsValid())
	{
		return SeasonController.Get();
	}

	USeasonSubsystem* Subsystem = USeasonSubsystem::Get(FMiscLibrary::GetCurrentWorld());
	if (IsValid(Subsystem))
	{
		USeasonController* Controller = Subsystem->GetSeasonController();
		SeasonController = TWeakObjectPtr<USeasonController>(Controller);
		return Controller;
	}

	return nullptr;
}

