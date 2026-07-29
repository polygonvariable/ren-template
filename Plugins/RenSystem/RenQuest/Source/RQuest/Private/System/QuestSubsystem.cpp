// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/QuestSubsystem.h"

// Project Headers
#include "Core/QuestSettings.h"
#include "Data/QuestAsset.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "System/Flow/QuestEngine.h"


void UQuestSubsystem::StartQuest(const FPrimaryAssetId& AssetId)
{
	if (!AssetId.IsValid() || ActiveQuests.Contains(AssetId))
	{
		LOG_ERROR(LogTemp, TEXT("AssetId is invalid or is already active"));
		return;
	}

	UQuestEngine* Engine = NewObject<UQuestEngine>(this);
	if (!IsValid(Engine))
	{
		LOG_ERROR(LogTemp, TEXT("Failed to create QuestEngine"));
		return;
	}

	FEventflowEntry Entry;
	Entry.EntryType = EEventflowEntryType::Root;

	Engine->OnStateChanged.BindUObject(this, &UQuestSubsystem::HandleOnStateChanged, AssetId);
	Engine->InitializeData(AssetId, Entry);
	Engine->Initialize();

	ActiveQuests.Add(AssetId, Engine);
	PRINT_SUCCESS(LogTemp, 1.0f, TEXT("Quest started, added to ActiveQuests"));
}


void UQuestSubsystem::CancelQuest(const FPrimaryAssetId& AssetId)
{
	TObjectPtr<UQuestEngine>* FoundEngine = ActiveQuests.Find(AssetId);
	if (!FoundEngine)
	{
		LOG_ERROR(LogTemp, TEXT("Failed to find QuestEngine"));
		return;
	}

	UQuestEngine* Engine = FoundEngine->Get();
	if (IsValid(Engine))
	{
		Engine->OnStateChanged.Unbind();

		if (Engine->GetState() == EFSMState::Active)
		{
			Engine->Finish(EFSMResult::Aborted);
		}
		if (Engine->GetState() != EFSMState::Uninitialized)
		{
			Engine->Reset();
		}
	}

	ActiveQuests.Remove(AssetId);
	PRINT_SUCCESS(LogTemp, 1.0f, TEXT("Quest canceled, removing from ActiveQuests"));
}




void UQuestSubsystem::HandleOnStateChanged(EFSMState PreviousState, EFSMState NewState, EFSMResult Result, FPrimaryAssetId AssetId)
{
	if (NewState == EFSMState::Finished)
	{
		TObjectPtr<UQuestEngine>* FoundEngine = ActiveQuests.Find(AssetId);
		if (!FoundEngine)
		{
			LOG_ERROR(LogTemp, TEXT("Failed to find QuestEngine"));
			return;
		}

		UQuestEngine* Engine = FoundEngine->Get();
		if (!IsValid(Engine))
		{
			LOG_ERROR(LogTemp, TEXT("Failed to get QuestEngine"));
			return;
		}

		Engine->OnStateChanged.Unbind();

		if (Engine->GetState() == EFSMState::Active)
		{
			Engine->Finish(EFSMResult::Aborted);
		}
		if (Engine->GetState() != EFSMState::Uninitialized)
		{
			Engine->Reset();
		}

		ActiveQuests.Remove(AssetId);
		PRINT_SUCCESS(LogTemp, 1.0f, TEXT("Quest ended, removing from ActiveQuests"));
	}
}





bool UQuestSubsystem::DoesSupportWorldType(EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

bool UQuestSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	UWorld* World = CastChecked<UWorld>(Outer);
	if (!DoesSupportWorldType(World->WorldType))
	{
		return false;
	}

	const UQuestSettings* Settings = UQuestSettings::Get();
	return GetClass() == Settings->SubsystemClass;
}

void UQuestSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogTemp, TEXT("QuestSubsystem initialized"));
}

void UQuestSubsystem::OnWorldComponentsUpdated(UWorld& InWorld)
{
	Super::OnWorldComponentsUpdated(InWorld);
}

void UQuestSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
}

void UQuestSubsystem::Deinitialize()
{
	for (const TPair<FPrimaryAssetId, TObjectPtr<UQuestEngine>>& Kv : ActiveQuests)
	{
		UQuestEngine* Engine = Kv.Value.Get();
		if (!IsValid(Engine))
		{
			continue;
		}

		Engine->OnStateChanged.Unbind();

		if (Engine->GetState() == EFSMState::Active)
		{
			Engine->Finish(EFSMResult::Aborted);
		}
		if (Engine->GetState() != EFSMState::Uninitialized)
		{
			Engine->Reset();
		}

		Engine->MarkAsGarbage();
	}

	ActiveQuests.Empty();

	LOG_WARNING(LogTemp, TEXT("QuestSubsystem deinitialized"));
	Super::Deinitialize();
}

UQuestSubsystem* UQuestSubsystem::Get(UWorld* World)
{
	if (!IsValid(World))
	{
		return nullptr;
	}
	return World->GetSubsystem<UQuestSubsystem>();
}

UQuestSubsystem* UQuestSubsystem::Get(UGameInstance* GameInstance)
{
	if (!IsValid(GameInstance))
	{
		return nullptr;
	}
	return Get(GameInstance->GetWorld());
}
