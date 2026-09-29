// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "DialogueSubsystem.h"

// Project Headers
#include "DialogueEngine.h"
#include "EventflowEngine.h"
#include "Library/PoolHelper.h"
#include "Log/LogMacro.h"


void UDialogueSubsystem::StartDialogue(const FPrimaryAssetId& AssetId)
{
	if (Dialogues.Contains(AssetId))
	{
		LOG_ERROR(LogTemp, TEXT("Dialogue is already active"));
		return;
	}

	UDialogueEngine* Dialogue = FPoolHelper::AcquireFromArray<UDialogueEngine>(EnginePool, UDialogueEngine::StaticClass(), this);
	if (!IsValid(Dialogue))
	{
		LOG_ERROR(LogTemp, TEXT("Failed to create dialogue"));
		return;
	}

	Dialogues.Add(AssetId, Dialogue);

	Dialogue->InitializeData(AssetId, FEventflowEntry());
	Dialogue->Initialize();
	Dialogue->OnStateChanged.BindUObject(this, &UDialogueSubsystem::HandleOnEngineStateChanged, AssetId);
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


void UDialogueSubsystem::HandleOnEngineStateChanged(EFSMState PreviousState, EFSMState NewState, EFSMResult Result, FPrimaryAssetId AssetId)
{
	if (NewState == EFSMState::Ready)
	{
		TObjectPtr<UDialogueEngine>* FoundEngine = Dialogues.Find(AssetId);
		if (FoundEngine)
		{
			UDialogueEngine* Engine = FoundEngine->Get();
			if (IsValid(Engine))
			{
				OnDialogueAdded.ExecuteIfBound(AssetId, Engine);
			}
		}
	}
	else if (NewState == EFSMState::Finished)
	{
		TObjectPtr<UDialogueEngine>* FoundEngine = Dialogues.Find(AssetId);
		if (FoundEngine)
		{
			UDialogueEngine* Engine = FoundEngine->Get();
			if (IsValid(Engine))
			{
				OnDialogueRemoved.ExecuteIfBound(AssetId, Engine);
				Engine->Reset();
			}
		}
	}
	else if (NewState == EFSMState::Uninitialized)
	{
		TObjectPtr<UDialogueEngine>* FoundEngine = Dialogues.Find(AssetId);
		if (FoundEngine)
		{
			UDialogueEngine* Engine = FoundEngine->Get();
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

