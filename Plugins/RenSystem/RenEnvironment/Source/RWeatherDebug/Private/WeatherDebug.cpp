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
#include "Actor/WeatherEffectManager.h"
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


void FWeatherDebugWidget::Draw_WeatherManager(AWeatherEffectManager* Manager)
{
	if (SlateIM::NextTableCell())
	{
		SlateIM::HAlign(HAlign_Fill);
		SlateIM::BeginHorizontalStack();
		{
			SlateIM::Text(TEXT("Manager:"));
			SlateIM::Text(Manager->GetFName().ToString(), FColor::Cyan);
		}
		SlateIM::EndHorizontalStack();
	}
}

void FWeatherDebugWidget::Draw_WeatherManagerEffects(AWeatherEffectManager* Manager)
{
	const TArray<TObjectPtr<AWeatherEffectActor>>& Effects = Manager->GetEditorWeatherEffects();

	if (SlateIM::NextTableCell())
	{
		SlateIM::HAlign(HAlign_Fill);
		SlateIM::BeginHorizontalStack();
		{
			SlateIM::Text(TEXT("Effects:"));
			SlateIM::Text(FString::FromInt(Effects.Num()), FColor::Cyan);
		}
		SlateIM::EndHorizontalStack();
		SlateIM::BeginTable();
		{
			SlateIM::BeginTableHeader();
			{
				SlateIM::InitialTableColumnWidth(150.0f);
				SlateIM::AddTableColumn(TEXT("Effect_Actor"), TEXT("Actor"));

				SlateIM::InitialTableColumnWidth(150.0f);
				SlateIM::AddTableColumn(TEXT("Effect_Asset"), TEXT("Asset"));

				SlateIM::InitialTableColumnWidth(150.0f);
				SlateIM::AddTableColumn(TEXT("Effect_State"), TEXT("State"));
			}
			SlateIM::EndTableHeader();

			SlateIM::BeginTableBody();
			{
				for (TObjectPtr<AWeatherEffectActor> Item : Effects)
				{
					SlateIM::NextTableCell();
					SlateIM::Text(IsValid(Item) ? Item->GetFName().ToString() : TEXT("Invalid Actor"));

					SlateIM::NextTableCell();
					SlateIM::Text(IsValid(Item) ? Item->EffectPath.ToString() : TEXT("Invalid Asset"));

					FString State = "Inactive";
					UActorComponent* Component = Item->GetEditorEffectComponent();
					if (IsValid(Component))
					{
						if (Component->IsActive())
						{
							State = TEXT("Active");
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

void FWeatherDebugWidget::Draw_WeatherManagerEffectHandles(AWeatherEffectManager* Manager)
{
	const TMap<TObjectPtr<UWeatherAsset>, TSharedPtr<FStreamableHandle>>& Effects = Manager->GetEditorWeatherEffectHandles();

	if (SlateIM::NextTableCell())
	{
		SlateIM::HAlign(HAlign_Fill);
		SlateIM::BeginHorizontalStack();
		{
			SlateIM::Text(TEXT("Effect Handles:"));
			SlateIM::Text(FString::FromInt(Effects.Num()), FColor::Cyan);
		}
		SlateIM::EndHorizontalStack();
		SlateIM::BeginTable();
		{
			SlateIM::BeginTableHeader();
			{
				SlateIM::InitialTableColumnWidth(150.0f);
				SlateIM::AddTableColumn(TEXT("EffectHandle_Asset"), TEXT("Asset"));

				SlateIM::InitialTableColumnWidth(150.0f);
				SlateIM::AddTableColumn(TEXT("EffectHandle_Handle"), TEXT("Handle"));
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
		SlateIM::Text(TEXT("Active:"));
		SlateIM::BeginTable();
		{
			SlateIM::BeginTableHeader();
			{
				SlateIM::InitialTableColumnWidth(150.0f);
				SlateIM::AddTableColumn(TEXT("Weather_Priority"), TEXT("Priority"));

				SlateIM::InitialTableColumnWidth(150.0f);
				SlateIM::AddTableColumn(TEXT("Weather_Name"), TEXT("Name"));

				SlateIM::InitialTableColumnWidth(150.0f);
				SlateIM::AddTableColumn(TEXT("Weather_Transition"), TEXT("Transition"));
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
		SlateIM::BeginHorizontalStack();
		{
			SlateIM::Text(TEXT("List:"));
			SlateIM::Text(FString::FromInt(Weathers.Num()), FColor::Cyan);
		}
		SlateIM::EndHorizontalStack();
		SlateIM::BeginTable();
		{
			SlateIM::BeginTableHeader();
			{
				SlateIM::InitialTableColumnWidth(150.0f);
				SlateIM::AddTableColumn(TEXT("Weather_Priority"), TEXT("Priority"));
				SlateIM::InitialTableColumnWidth(150.0f);
				SlateIM::AddTableColumn(TEXT("Weather_Asset"), TEXT("Asset"));
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
	AWeatherEffectManager* Manager = GetWeatherManager();
	UWeatherController* Controller = GetController();

	SlateIM::FWindowParams WindowParams;
	WindowParams.WindowSize = FVector2f(150.0f * 3, 250.0f * 1.5);
	WindowParams.bAlwaysOnTop = true;

	if (SlateIM::BeginWindowRoot(TEXT("Weather_Window"), WindowParams))
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
					SlateIM::Text(TEXT("Manager:"));
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
					SlateIM::Text(TEXT("Controller:"));
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

AWeatherEffectManager* FWeatherDebugWidget::GetWeatherManager()
{
	if (WeatherManager.IsValid())
	{
		return WeatherManager.Get();
	}

	UWorld* World = FMiscLibrary::GetCurrentWorld();
	if (IsValid(World))
	{
		for (TActorIterator<AWeatherEffectManager> ActorItr(World); ActorItr; ++ActorItr)
		{
			WeatherManager = TWeakObjectPtr<AWeatherEffectManager>(*ActorItr);
		}
	}

	return nullptr;
}
