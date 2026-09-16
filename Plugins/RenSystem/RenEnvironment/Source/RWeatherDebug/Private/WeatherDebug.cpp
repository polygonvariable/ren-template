// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "WeatherDebug.h"

// Engine Headers
#include "Engine/AssetManager.h"
#include "EngineUtils.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "SlateIM.h"

// Project Headers
#include "Actor/WeatherEffectActor.h"
#include "Actor/WeatherEffectManagerActor.h"
#include "Data/WeatherAsset.h"
#include "MiscLibrary.h"
#include "System/WeatherController.h"
#include "System/WeatherSubsystem.h"


void FWeatherDebugWidget::EnableWidget()
{
	FSlateIMWidgetBase::EnableWidget();
}

void FWeatherDebugWidget::DisableWidget()
{
	WeatherManager.Reset();
	WeatherController.Reset();

	FSlateIMWidgetBase::DisableWidget();
}


void FWeatherDebugWidget::Draw_WeatherManager(AWeatherEffectManagerActor* Manager)
{
	if (SlateIM::NextTableCell())
	{
		SlateIM::HAlign(HAlign_Fill);
		SlateIM::BeginHorizontalStack();
		{
			SlateIM::Text(TEXT("Weather Manager:"));
			SlateIM::Text(Manager->GetFName().ToString(), FColor::Cyan);
		}
		SlateIM::EndHorizontalStack();
	}
}

void FWeatherDebugWidget::Draw_WeatherManagerEffects(AWeatherEffectManagerActor* Manager)
{
	const TArray<TObjectPtr<AWeatherEffectActor>>& Effects = Manager->GetEditorWeatherEffects();

	if (SlateIM::NextTableCell())
	{
		SlateIM::HAlign(HAlign_Fill);
		SlateIM::BeginHorizontalStack();
		{
			SlateIM::Text(TEXT("Weather Effects:"));
			SlateIM::Text(FString::FromInt(Effects.Num()), FColor::Cyan);
		}
		SlateIM::EndHorizontalStack();
		SlateIM::BeginTable();
		{
			SlateIM::BeginTableHeader();
			{
				SlateIM::InitialTableColumnWidth(150.0f);
				SlateIM::AddTableColumn(TEXT("Weather_ListEffect_Actor"), TEXT("Actor"));

				SlateIM::InitialTableColumnWidth(150.0f);
				SlateIM::AddTableColumn(TEXT("Weather_ListEffect_Asset"), TEXT("Asset"));

				SlateIM::InitialTableColumnWidth(150.0f);
				SlateIM::AddTableColumn(TEXT("Weather_ListEffect_State"), TEXT("State"));
			}
			SlateIM::EndTableHeader();

			SlateIM::BeginTableBody();
			{
				for (TObjectPtr<AWeatherEffectActor> Item : Effects)
				{
					SlateIM::NextTableCell();
					SlateIM::Text(IsValid(Item) ? Item->GetFName().ToString() : TEXT("Invalid Actor"));

					UNiagaraSystem* System = Item->GetNiagaraSystem();
					SlateIM::NextTableCell();
					SlateIM::Text(IsValid(System) ? Item->GetFName().ToString() : TEXT("Invalid Niagara"));

					FString State = "Inactive";
					UNiagaraComponent* Component = Item->GetEditorNiagaraComponent();
					if (IsValid(Component))
					{
						if (Component->IsActive())
						{
							State = TEXT("Active");
						}
						else if (Component->IsPaused())
						{
							State = TEXT("Paused");
						}
					}
					SlateIM::NextTableCell();
					SlateIM::Text(State);
				}
			}
			SlateIM::EndTableBody();
		}
		SlateIM::EndTable();
	}
}

void FWeatherDebugWidget::Draw_WeatherManagerEffectHandles(AWeatherEffectManagerActor* Manager)
{
	const TMap<TObjectPtr<UWeatherAsset>, TSharedPtr<FStreamableHandle>>& Effects = Manager->GetEditorWeatherEffectHandles();

	if (SlateIM::NextTableCell())
	{
		SlateIM::HAlign(HAlign_Fill);
		SlateIM::BeginHorizontalStack();
		{
			SlateIM::Text(TEXT("Weather Effects Handles:"));
			SlateIM::Text(FString::FromInt(Effects.Num()), FColor::Cyan);
		}
		SlateIM::EndHorizontalStack();
		SlateIM::BeginTable();
		{
			SlateIM::BeginTableHeader();
			{
				SlateIM::InitialTableColumnWidth(150.0f);
				SlateIM::AddTableColumn(TEXT("Weather_ListEffectHandle_Asset"), TEXT("Asset"));

				SlateIM::InitialTableColumnWidth(150.0f);
				SlateIM::AddTableColumn(TEXT("Weather_ListEffectHandle_Item"), TEXT("Handle"));
			}
			SlateIM::EndTableHeader();

			SlateIM::BeginTableBody();
			{
				for (const TPair<TObjectPtr<UWeatherAsset>, TSharedPtr<FStreamableHandle>>& Kv : Effects)
				{
					SlateIM::NextTableCell();
					SlateIM::Text(IsValid(Kv.Key) ? Kv.Key->GetFName().ToString() : TEXT("Invalid Asset"));

					SlateIM::NextTableCell();
					SlateIM::Text(Kv.Value.IsValid() ? Kv.Value->GetDebugName() : TEXT("Invalid Handle"));
				}
			}
			SlateIM::EndTableBody();
		}
		SlateIM::EndTable();
	}
}


void FWeatherDebugWidget::Draw_WeatherControllerActive(UWeatherController* Controller)
{
	if (SlateIM::NextTableCell())
	{
		SlateIM::HAlign(HAlign_Fill);
		SlateIM::Text(TEXT("Weather Active:"));
		SlateIM::BeginTable();
		{
			SlateIM::BeginTableHeader();
			{
				SlateIM::InitialTableColumnWidth(150.0f);
				SlateIM::AddTableColumn(TEXT("Weather_CurrentPriority"), TEXT("Priority"));

				SlateIM::InitialTableColumnWidth(150.0f);
				SlateIM::AddTableColumn(TEXT("Weather_CurrentAsset"), TEXT("Weather"));

				SlateIM::InitialTableColumnWidth(150.0f);
				SlateIM::AddTableColumn(TEXT("Weather_CurrentTransition"), TEXT("Transition"));
			}
			SlateIM::EndTableHeader();

			SlateIM::BeginTableBody();
			{
				SlateIM::NextTableCell();
				SlateIM::Text(FString::FromInt(Controller->GetEditorCurrentWeatherPriority()));

				SlateIM::NextTableCell();
				SlateIM::Text(Controller->GetEditorCurrentWeatherName());

				SlateIM::NextTableCell();
				SlateIM::Text(FString::Printf(TEXT("%f"), Controller->GetEditorWeatherTransition()));
			}
			SlateIM::EndTableBody();
		}
		SlateIM::EndTable();
	}
}

void FWeatherDebugWidget::Draw_WeatherControllerList(UWeatherController* Controller)
{
	const TMap<int, TWeakObjectPtr<UObject>>& Weathers = Controller->GetEditorWeathers();

	if (SlateIM::NextTableCell())
	{
		SlateIM::HAlign(HAlign_Fill);
		SlateIM::Text(TEXT("Weather List: (" + FString::FromInt(Weathers.Num()) + ")"));
		SlateIM::BeginTable();
		{
			SlateIM::BeginTableHeader();
			{
				SlateIM::InitialTableColumnWidth(150.0f);
				SlateIM::AddTableColumn(TEXT("Weather_ListPriority"), TEXT("Priority"));
				SlateIM::InitialTableColumnWidth(150.0f);
				SlateIM::AddTableColumn(TEXT("Weather_ListAsset"), TEXT("Weather"));
			}
			SlateIM::EndTableHeader();

			SlateIM::BeginTableBody();
			{
				for (const TPair<int, TWeakObjectPtr<UObject>>& Kv : Weathers)
				{
					SlateIM::NextTableCell();
					SlateIM::Text(FString::FromInt(Kv.Key));

					UObject* WeatherAsset = Kv.Value.Get();

					SlateIM::NextTableCell();
					SlateIM::Text(IsValid(WeatherAsset) ? WeatherAsset->GetFName().ToString() : TEXT("Invalid Asset"));
				}
			}
			SlateIM::EndTableBody();
		}
		SlateIM::EndTable();
	}
}


void FWeatherDebugWidget::DrawWidget(float DeltaTime)
{
	AWeatherEffectManagerActor* Manager = GetWeatherManager();
	UWeatherController* Controller = GetController();

	SlateIM::FWindowParams WindowParams;
	WindowParams.WindowSize = FVector2f(150.0f * 2, 250.0f * 1.5);
	WindowParams.bAlwaysOnTop = true;

	if (SlateIM::BeginWindowRoot(TEXT("WeatherWindow"), WindowParams))
	{
		SlateIM::BeginTable();
		SlateIM::AddTableColumn(TEXT("Weather_Debug"), TEXT("Weather Debugger:"));

		if (Manager)
		{
			Draw_WeatherManager(Manager);
			Draw_WeatherManagerEffects(Manager);
			Draw_WeatherManagerEffectHandles(Manager);
		}
		else
		{
			if (SlateIM::NextTableCell())
			{
				SlateIM::BeginHorizontalStack();
				{
					SlateIM::Text(TEXT("Weather Manager:"));
					SlateIM::Text(TEXT("Invalid"), FColor::Red);
				}
				SlateIM::EndHorizontalStack();
			}
		}

		if (Controller)
		{
			Draw_WeatherControllerActive(Controller);
			Draw_WeatherControllerList(Controller);
		}
		else
		{
			if (SlateIM::NextTableCell())
			{
				SlateIM::BeginHorizontalStack();
				{
					SlateIM::Text(TEXT("Weather Controller:"));
					SlateIM::Text(TEXT("Invalid"), FColor::Red);
				}
				SlateIM::EndHorizontalStack();
			}
		}
			
		SlateIM::EndTable();
	}
	SlateIM::EndRoot();
}

UWeatherController* FWeatherDebugWidget::GetController()
{
	if (WeatherController.IsValid())
	{
		return WeatherController.Get();
	}

	UWeatherSubsystem* Subsystem = UWeatherSubsystem::Get(FMiscLibrary::GetCurrentWorld());
	if (IsValid(Subsystem))
	{
		UWeatherController* Controller = Subsystem->GetWeatherController();

		WeatherController = TWeakObjectPtr<UWeatherController>(Controller);
		return Controller;
	}

	return nullptr;
}

AWeatherEffectManagerActor* FWeatherDebugWidget::GetWeatherManager()
{
	if (WeatherManager.IsValid())
	{
		return WeatherManager.Get();
	}

	UWorld* World = FMiscLibrary::GetCurrentWorld();
	if (IsValid(World))
	{
		for (TActorIterator<AWeatherEffectManagerActor> ActorItr(World); ActorItr; ++ActorItr)
		{
			WeatherManager = TWeakObjectPtr<AWeatherEffectManagerActor>(*ActorItr);
		}
	}

	return nullptr;
}
