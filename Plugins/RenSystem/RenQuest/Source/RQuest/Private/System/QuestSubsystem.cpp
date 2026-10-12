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
#include "System/Flow/Task/QuestPrimaryTask.h"


void UQuestSubsystem::StartEventflow(const FPrimaryAssetId& AssetId, const TInstancedStruct<FEventflowEntryData>& EntryData)
{
	StartQuest(AssetId, EntryData);
}

void UQuestSubsystem::StopEventflow(const FPrimaryAssetId& AssetId)
{

}

void UQuestSubsystem::StartQuest(const FPrimaryAssetId& AssetId, const TInstancedStruct<FEventflowEntryData>& EntryData)
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

	Engine->OnStateChanged.BindUObject(this, &UQuestSubsystem::HandleOnQuestStateChanged, AssetId);
	Engine->InitializeData(AssetId, EntryData);
	Engine->Initialize();

	PRINT_SUCCESS(LogTemp, 1.0f, TEXT("Quest started, added to Quests"));
}



void UQuestSubsystem::FocusQuest(FPrimaryAssetId AssetId)
{
	TObjectPtr<UQuestEngine>* FoundQEngine = Quests.Find(AssetId);
	if (!FoundQEngine) return;

	UQuestEngine* QEngine = FoundQEngine->Get();
	if (!QEngine) return;

	const FEventflowEntryData* QData = QEngine->GetEntryData().GetPtr<FEventflowEntryData>();
	if (!QData) return;

	UQuestEngine* ParentEngine = nullptr;

	if (QData->GroupId.IsValid())
	{
		TObjectPtr<UQuestEngine>* FoundParentEngine = Quests.Find(QData->GroupId);
		if (!FoundParentEngine) return;

		ParentEngine = FoundParentEngine->Get();
	}
	else
	{
		ParentEngine = QEngine;
	}

	if (FocusedQuest == ParentEngine)
	{
		FocusedQuest.Reset();
		OnQuestFocusChanged.Broadcast();
		return;
	}

	FocusedQuest = TWeakObjectPtr<UQuestEngine>(ParentEngine);
	OnQuestFocusChanged.Broadcast();
}

void UQuestSubsystem::CancelFocusedTask()
{
	UQuestEngine* Engine = GetFocusedQuest();
	if (!Engine)
	{
		return;
	}
	const UQuestAsset* Asset = Engine->GetAsset<UQuestAsset>();
	if (!Asset)
	{
		return;
	}

	CancelQuestTask(Asset->GetPrimaryAssetId(), true);
}

void UQuestSubsystem::CancelQuestTask(FPrimaryAssetId AssetId)
{
	TObjectPtr<UQuestEngine>* FoundEngine = Quests.Find(AssetId);
	if (!FoundEngine) return;

	UQuestEngine* Engine = FoundEngine->Get();
	if (!Engine) return;

	UQuestPrimaryTask* Node = Engine->GetNodeTask<UQuestPrimaryTask>();
	if (!Node) return;

	if (Node->bCanCancel)
	{
		Node->Finish(EFSMResult::Cancelled);
	}
}

void UQuestSubsystem::CancelQuestTask(FPrimaryAssetId AssetId, bool bRecursive)
{
	TObjectPtr<UQuestEngine>* FoundQEngine = Quests.Find(AssetId);
	if (!FoundQEngine) return;

	UQuestEngine* QEngine = FoundQEngine->Get();
	if (!QEngine) return;

	const FEventflowEntryData* QData = QEngine->GetEntryData().GetPtr<FEventflowEntryData>();
	if (!QData) return;

	UQuestEngine* ParentEngine = nullptr;

	if (QData->GroupId.IsValid())
	{
		TObjectPtr<UQuestEngine>* FoundParentEngine = Quests.Find(QData->GroupId);
		if (!FoundParentEngine) return;

		ParentEngine = FoundParentEngine->Get();
	}
	else
	{
		ParentEngine = QEngine;
	}

	if (!ParentEngine) return;

	UQuestPrimaryTask* ParentNode = ParentEngine->GetNodeTask<UQuestPrimaryTask>();
	if (!ParentNode) return;

	if (ParentNode->bCanCancel)
	{
		ParentNode->Finish(EFSMResult::Cancelled);
		return;
	}

	TMap<FPrimaryAssetId, TObjectPtr<UQuestEngine>> GroupQuests;
	GetQuestsByGroup(ParentEngine->GetAsset()->GetPrimaryAssetId(), GroupQuests);

	for (TPair<FPrimaryAssetId, TObjectPtr<UQuestEngine>>& Kv : GroupQuests)
	{
		UQuestEngine* GroupEngine = Kv.Value.Get();
		if (!GroupEngine) return;

		UQuestPrimaryTask* GroupNode = GroupEngine->GetNodeTask<UQuestPrimaryTask>();
		if (!GroupNode) return;

		if (GroupNode->bCanCancel)
		{
			GroupNode->Finish(EFSMResult::Cancelled);
			return;
		}
	}
}




UQuestEngine* UQuestSubsystem::GetFocusedQuest() const
{
	return FocusedQuest.Get();
}

const TMap<FPrimaryAssetId, TObjectPtr<UQuestEngine>>& UQuestSubsystem::GetQuests() const
{
	return Quests;
}

void UQuestSubsystem::GetQuestsByGroup(const FPrimaryAssetId& InAssetId, TMap<FPrimaryAssetId, TObjectPtr<UQuestEngine>>& OutQuests) const
{
	for (const TPair<FPrimaryAssetId, TObjectPtr<UQuestEngine>>& Kv : Quests)
	{
		UQuestEngine* Engine = Kv.Value.Get();
		if (!Engine)
		{
			continue;
		}

		const FEventflowEntryData* Data = Engine->GetEntryData().GetPtr<FEventflowEntryData>();
		if (Data && Data->GroupId == InAssetId)
		{
			OutQuests.Add(Kv);
		}
	}
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
		{
			OnEngineAdded.Broadcast(AssetId, Engine);
			Engine->Active();

			UQuestAsset* Asset = Engine->GetAsset<UQuestAsset>();
			if (Asset->bAutoFocus)
			{
				FocusedQuest = Engine;
				OnQuestFocusChanged.Broadcast();
			}
		}
		break;

	case EFSMState::Finished:
		if (FocusedQuest == Engine)
		{
			FocusedQuest.Reset();
			OnQuestFocusChanged.Broadcast();
		}
		OnEngineRemoved.Broadcast(AssetId, Engine);
		Engine->Reset();
		break;

	case EFSMState::Uninitialized:
		Engine->OnStateChanged.Unbind();
		if (Quests.Remove(AssetId) > 0)
		{
			FPoolLibrary::ReturnToArray(EnginePool, Engine);
		}
		OnQuestDataChanged.Broadcast();
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

