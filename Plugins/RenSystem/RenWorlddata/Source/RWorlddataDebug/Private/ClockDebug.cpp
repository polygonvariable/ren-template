// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "ClockDebug.h"

// Engine Headers
#include "SlateIM.h"

// Project Headers
#include "Clock/ClockSubsystem.h"
#include "MiscLibrary.h"


void FClockDebugWidget::DisableWidget()
{
	ClockSubsystem.Reset();
	FSlateIMWidgetBase::DisableWidget();
}

void FClockDebugWidget::DrawWidget(float DeltaTime)
{
	SlateIM::FWindowParams WindowParams;
	WindowParams.WindowSize = FVector2f(150.0f * 2, 250.0f * 1.5);
	WindowParams.bAlwaysOnTop = true;

	if (SlateIM::BeginWindowRoot(TEXT("Clock_Window"), WindowParams))
	{
		SlateIM::BeginTable();
		SlateIM::AddTableColumn(TEXT("Clock_Debug"), TEXT("Clock Debugger:"));

		UClockSubsystem* Subsystem = GetClockSubsystem();
		if (IsValid(Subsystem))
		{
			if (SlateIM::NextTableCell())
			{
				SlateIM::Text(TEXT("Definition:"));
				SlateIM::BeginTable();
				{
					SlateIM::BeginTableHeader();
					{
						SlateIM::InitialTableColumnWidth(100.0f);
						SlateIM::AddTableColumn(TEXT("Clock_Time"), TEXT("Day Length"));
						SlateIM::InitialTableColumnWidth(100.0f);
						SlateIM::AddTableColumn(TEXT("Clock_Day"), TEXT("Year Length"));
					}
					SlateIM::EndTableHeader();
					SlateIM::BeginTableBody();
					{
						int DayLength = 1;
						Subsystem->GetDayLength(DayLength);
						SlateIM::NextTableCell();
						SlateIM::Text(FString::FromInt(DayLength), FColor::Cyan);

						int YearLength = 1;
						Subsystem->GetYearLength(YearLength);
						SlateIM::NextTableCell();
						SlateIM::Text(FString::FromInt(YearLength), FColor::Cyan);
					}
					SlateIM::EndTableBody();
				}
				SlateIM::EndTable();
			}

			if (SlateIM::NextTableCell())
			{
				SlateIM::Text(TEXT("Runtime:"));
				SlateIM::BeginTable();
				{
					SlateIM::BeginTableHeader();
					{
						SlateIM::InitialTableColumnWidth(100.0f);
						SlateIM::AddTableColumn(TEXT("Clock_Time"), TEXT("Time"));
						SlateIM::InitialTableColumnWidth(100.0f);
						SlateIM::AddTableColumn(TEXT("Clock_Day"), TEXT("Day"));
						SlateIM::InitialTableColumnWidth(100.0f);
						SlateIM::AddTableColumn(TEXT("Clock_Year"), TEXT("Year"));
					}
					SlateIM::EndTableHeader();
					SlateIM::BeginTableBody();
					{
						SlateIM::NextTableCell();
						SlateIM::Text(FString::FromInt(Subsystem->GetCurrentTime()), FColor::Cyan);

						SlateIM::NextTableCell();
						SlateIM::Text(FString::FromInt(Subsystem->GetCurrentDay()), FColor::Cyan);

						SlateIM::NextTableCell();
						SlateIM::Text(FString::FromInt(Subsystem->GetCurrentYear()), FColor::Cyan);
					}
					SlateIM::EndTableBody();
				}
				SlateIM::EndTable();
			}
		}
		else
		{
			if (SlateIM::NextTableCell())
			{
				SlateIM::Text(TEXT("Clock Invalid"), FColor::Red);
			}
		}

		SlateIM::EndTable();
	}
	SlateIM::EndRoot();
}

UClockSubsystem* FClockDebugWidget::GetClockSubsystem()
{
	UClockSubsystem* Subsystem = ClockSubsystem.Get();
	if (!IsValid(Subsystem))
	{
		ClockSubsystem = TWeakObjectPtr<UClockSubsystem>(UClockSubsystem::Get(FMiscLibrary::GetCurrentWorld()));
	}
	return Subsystem;
}

