// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/QuestSubsystem.h"

// Project Headers
#include "Core/PoolLibrary.h"
#include "Core/QuestSettings.h"
#include "Data/QuestAsset.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "LuauSubsystem.h"
#include "System/Flow/QuestEngine.h"
#include "lua.h"
#include "luacode.h"
#include "lualib.h"
#include "MiscLibrary.h"


void UQuestSubsystem::StartEventflow(const FPrimaryAssetId& AssetId)
{
	StartQuest(AssetId);
}

void UQuestSubsystem::StopEventflow(const FPrimaryAssetId& AssetId)
{
	CancelQuest(AssetId);
}

void UQuestSubsystem::StartQuest(const FPrimaryAssetId& AssetId)
{
	if (!AssetId.IsValid() || Quests.Contains(AssetId))
	{
		LOG_ERROR(LogTemp, TEXT("AssetId is invalid or is already active"));
		return;
	}

	UQuestEngine* Engine = FPoolLibrary::AcquireFromArray<UQuestEngine>(EnginePool, UQuestEngine::StaticClass(), this);
	if (!ensureAlwaysMsgf(IsValid(Engine), TEXT("Failed to acquire/create quest engine")))
	{
		return;
	}

	Quests.Add(AssetId, Engine);

	FEventflowEntryData EntryData;
	EntryData.EntryType = EEventflowEntryType::Root;

	Engine->OnStateChanged.BindUObject(this, &UQuestSubsystem::HandleOnQuestStateChanged, AssetId);
	Engine->InitializeData(AssetId, EntryData);
	Engine->Initialize();

	PRINT_SUCCESS(LogTemp, 1.0f, TEXT("Quest started, added to Quests"));
}


void UQuestSubsystem::CancelQuest(const FPrimaryAssetId& AssetId)
{
	UQuestEngine* Engine = GetQuestEngine(AssetId);
	if (IsValid(Engine))
	{
		UQuestAsset* QuestAsset = Engine->GetAsset<UQuestAsset>();
		if (IsValid(QuestAsset) && QuestAsset->bCanCancel)
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
	}

	Quests.Remove(AssetId);
	PRINT_SUCCESS(LogTemp, 1.0f, TEXT("Quest canceled, removing from Quests"));
}



#if UE_BUILD_DEVELOPMENT
const TMap<FPrimaryAssetId, TObjectPtr<UQuestEngine>>& UQuestSubsystem::GetEditorQuests() const
{
	return Quests;
}
#endif




void UQuestSubsystem::RemoveQuests()
{
	for (const TPair<FPrimaryAssetId, TObjectPtr<UQuestEngine>>& Kv : Quests)
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

	Quests.Empty();
}

UQuestEngine* UQuestSubsystem::GetQuestEngine(const FPrimaryAssetId& AssetId) const
{
	const TObjectPtr<UQuestEngine>* FoundEngine = Quests.Find(AssetId);
	if (FoundEngine)
	{
		return FoundEngine->Get();
	}
	return nullptr;
}

void UQuestSubsystem::HandleOnQuestStateChanged(EFSMState PreviousState, EFSMState NewState, EFSMResult Result, FPrimaryAssetId AssetId)
{
	if (NewState != EFSMState::Ready && NewState != EFSMState::Finished && NewState != EFSMState::Uninitialized)
	{
		return;
	}

	UQuestEngine* Engine = GetQuestEngine(AssetId);
	if (!IsValid(Engine))
	{
		return;
	}

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	switch (NewState)
	{
	case EFSMState::Ready:
		OnEngineAdded.Broadcast(AssetId, Engine);
		Engine->Active();
		break;

	case EFSMState::Finished:
		OnEngineRemoved.Broadcast(AssetId, Engine);
		Engine->Reset();
		break;

	case EFSMState::Uninitialized:
		Engine->OnStateChanged.Unbind();
		if (Quests.Remove(AssetId) > 0)
		{
			FPoolLibrary::ReturnToArray(EnginePool, Engine);
		}
		break;
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

void UQuestSubsystem::Deinitialize()
{
	RemoveQuests();

	LOG_WARNING(LogTemp, TEXT("QuestSubsystem deinitialized"));
	Super::Deinitialize();
}

UQuestSubsystem* UQuestSubsystem::Get(UWorld* World)
{
	check(IsValid(World));
	return World->GetSubsystem<UQuestSubsystem>();
}

