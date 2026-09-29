// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "DialogueSubsystem.h"

// Project Headers
#include "DialogueEngine.h"
#include "EventflowEngine.h"
#include "Library/PoolHelper.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "GameplayModeProvider.h"
#include "Util/SubsystemUtil.h"
#include "DialogueSettings.h"


void UDialogueSubsystem::StartDialogue(const FPrimaryAssetId& AssetId)
{
	if (Dialogues.Contains(AssetId))
	{
		LOG_ERROR(LogDialogue, TEXT("Dialogue is already active"));
		return;
	}

	UDialogueEngine* Dialogue = FPoolHelper::AcquireFromArray<UDialogueEngine>(EnginePool, UDialogueEngine::StaticClass(), this);
	if (!IsValid(Dialogue))
	{
		LOG_ERROR(LogDialogue, TEXT("Failed to create dialogue"));
		return;
	}

	Dialogues.Add(AssetId, Dialogue);

	Dialogue->OnStateChanged.BindUObject(this, &UDialogueSubsystem::HandleOnEngineStateChanged, AssetId);
	Dialogue->InitializeData(AssetId, FEventflowEntry());
	Dialogue->Initialize();
}

#if UE_BUILD_DEVELOPMENT
const TMap<FPrimaryAssetId, TObjectPtr<UDialogueEngine>>& UDialogueSubsystem::GetEditorDialogues() const
{
	return Dialogues;
}

int UDialogueSubsystem::GetEditorDialoguePoolSize() const
{
	return EnginePool.Num();
}
#endif

UDialogueEngine* UDialogueSubsystem::GetDialogueEngine(FPrimaryAssetId AssetId) const
{
	const TObjectPtr<UDialogueEngine>* FoundEngine = Dialogues.Find(AssetId);
	if (FoundEngine)
	{
		return FoundEngine->Get();
	}
	return nullptr;
}

void UDialogueSubsystem::HandleOnEngineStateChanged(EFSMState PreviousState, EFSMState NewState, EFSMResult Result, FPrimaryAssetId AssetId)
{
	if (NewState == EFSMState::Ready)
	{
		UDialogueEngine* Engine = GetDialogueEngine(AssetId);
		if (IsValid(Engine))
		{
			OnDialogueAdded.ExecuteIfBound(AssetId, Engine);
			Engine->Active();
		}
	}
	else if (NewState == EFSMState::Finished)
	{
		UDialogueEngine* Engine = GetDialogueEngine(AssetId);
		if (IsValid(Engine))
		{
			OnDialogueRemoved.ExecuteIfBound(AssetId);
			Engine->Reset();
		}
	}
	else if (NewState == EFSMState::Uninitialized)
	{
		UDialogueEngine* Engine = GetDialogueEngine(AssetId);
		if (IsValid(Engine))
		{
			Engine->OnStateChanged.Unbind();

			if (Dialogues.Remove(AssetId) > 0)
			{
				FPoolHelper::ReturnToArray(EnginePool, Engine);
			}
		}
	}
}

bool UDialogueSubsystem::DoesSupportWorldType(EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

bool UDialogueSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

UDialogueSubsystem* UDialogueSubsystem::Get(UWorld* World)
{
	return World->GetSubsystem<UDialogueSubsystem>();
}

