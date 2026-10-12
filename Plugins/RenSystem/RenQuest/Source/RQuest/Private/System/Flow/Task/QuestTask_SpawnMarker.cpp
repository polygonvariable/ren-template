// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/Flow/Task/QuestTask_SpawnMarker.h"

// Engine Headers
#include "UObject/ObjectSaveContext.h"

// Project Headers
#if WITH_EDITOR
#include "Core/QuestSettings.h"
#endif
#include "Actor/QuestObjectiveActor.h"
#include "ComponentTemplateLibrary.h"
#include "Log/LogMacro.h"
#include "LuauLibrary.h"
#include "LuauProperty.h"
#include "LuauSubsystem.h"
#include "MiscLibrary.h"



UQuestTask_SpawnMarker::UQuestTask_SpawnMarker()
{
}

void UQuestTask_SpawnMarker::CopyFromAsset(const UEventflowTask* Template)
{
	Super::CopyFromAsset(Template);
	
	const UQuestTask_SpawnMarker* Task = Cast<UQuestTask_SpawnMarker>(Template);
	checkf(IsValid(Task), TEXT("Node have invalid task template"));

	MarkerClass = Task->MarkerClass;
	MarkerTransform = Task->MarkerTransform;
}

#if WITH_EDITOR
void UQuestTask_SpawnMarker::AppendAssetBundleData(FAssetBundleData& AssetBundle)
{
	Super::AppendAssetBundleData(AssetBundle);

	const UQuestSettings* Settings = UQuestSettings::Get();
	const FName& BundleName = Settings->BundleName;

	if (!MarkerClass.IsNull())
	{
		AssetBundle.AddBundleAsset(BundleName, MarkerClass.ToSoftObjectPath().GetAssetPath());
	}
}
#endif

void UQuestTask_SpawnMarker::HandleOnInteractionCompleted(EFSMResult Result)
{
	Active();
}


void UQuestTask_SpawnMarker::OnInitialized(EFSMState PreviousState)
{
	Super::OnInitialized(PreviousState);
	FMiscLibrary::NextFrame(this, &UQuestTask_SpawnMarker::Load);
}

void UQuestTask_SpawnMarker::OnLoaded(EFSMState PreviousState)
{
	UQuestTask_SpawnMarker* Template = GetOwningTemplate<UQuestTask_SpawnMarker>();
	checkf(IsValid(Template), TEXT("Node have invalid task template"));

	MarkerActor = GetWorld()->SpawnActorDeferred<AQuestObjectiveMarker>(MarkerClass.Get(), MarkerTransform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	if (IsValid(MarkerActor))
	{
		MarkerActor->OnInteractionCompleted.BindUObject(this, &UQuestTask_SpawnMarker::HandleOnInteractionCompleted);
		MarkerActor->FinishSpawning(MarkerTransform);

		FComponentTemplateLibrary::RegisterComponents(MarkerActor, Template->MarkerComponents);

		FMiscLibrary::NextFrame(this, &UQuestTask_SpawnMarker::Ready);
	}
}

void UQuestTask_SpawnMarker::OnReady(EFSMState PreviousState)
{
	checkf(IsValid(MarkerActor), TEXT("Spawned actor is invalid"));
	
	MarkerActor->SetActorHiddenInGame(false);
	MarkerActor->SetActorEnableCollision(true);
}

void UQuestTask_SpawnMarker::OnActive(EFSMState PreviousState)
{
	Finish(EFSMResult::Success);
}

void UQuestTask_SpawnMarker::OnFinished(EFSMResult Result)
{
	if (IsValid(MarkerActor))
	{
		MarkerActor->SetActorHiddenInGame(true);
		MarkerActor->SetActorEnableCollision(false);
	}
}

void UQuestTask_SpawnMarker::OnReset()
{
	if (IsValid(MarkerActor))
	{
		MarkerActor->Destroy();
	}
	MarkerActor = nullptr;

	Super::OnReset();
}


void UQuestTask_ConditionalSpawnMarker::PreSave(FObjectPreSaveContext ObjectSaveContext)
{
#if WITH_EDITOR
	if (bAutoCompile)
	{
		if (!FLuauLibrary::Compile(LuauCode.Code, LuauCode.Bytecode))
		{
			LuauCode.Bytecode.Empty();
		}
	}
#endif

	Super::PreSave(ObjectSaveContext);
}

void UQuestTask_ConditionalSpawnMarker::HandleOnInteractionCompleted(EFSMResult Result)
{
	const UQuestTask_ConditionalSpawnMarker* Template = GetOwningTemplate<UQuestTask_ConditionalSpawnMarker>();
	ULuauSubsystem* LuauSubsystem = ULuauSubsystem::Get(GetWorld());
	if (!IsValid(Template) || !IsValid(LuauSubsystem))
	{
		return;
	}

	FLuauProperties Inputs;
	FLuauProperties Outputs;

	FLuauProperty_Table& InputTable = Inputs.SetTable();

	for (const TInstancedStruct<FQuestConditionDefinition>& Condition : Template->Conditions)
	{
		const FQuestConditionDefinition* Definition = Condition.GetPtr<FQuestConditionDefinition>();
		if (Definition)
		{
			InputTable.Set(Definition->Name.ToString(), Definition->Evaluate(GetWorld(), this));
		}
	}

	bool bSuccess = LuauSubsystem->ExecuteBytecode(Template->LuauCode.Bytecode, TEXT("script"), TEXT("evaluateCondition"), Inputs, Outputs);
	if (!bSuccess)
	{
		LOG_ERROR(LogTemp, TEXT("Failed to execute quest luau bytecode"));
		return;
	}

	const FLuauProperty_Boolean* OutputResult = Outputs.Get<FLuauProperty_Boolean>(0);
	if (!OutputResult)
	{
		LOG_ERROR(LogTemp, TEXT("Failed to get output from luau method"));
		return;
	}

	if (OutputResult->bValue)
	{
		TWeakObjectPtr<UQuestTask_ConditionalSpawnMarker> WeakThis(this);

		FTimerManager& TimerManager = GetWorld()->GetTimerManager();
		TimerManager.SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this,
			[WeakThis]()
			{
				UQuestTask_ConditionalSpawnMarker* This = WeakThis.Get();
				if (IsValid(This))
				{
					This->Active();
				}
			}
		));
	}
}



bool FQuestConditionDefinition::Evaluate(UWorld* World, UQuestPrimaryTask* Task) const
{
	return false;
}

bool FQCondition_ActorHaveTag::Evaluate(UWorld* World, UQuestPrimaryTask* Task) const
{
	APlayerController* PC = World->GetFirstPlayerController();
	if (PC)
	{
		APawn* Pawn = PC->GetPawn();
		if (Pawn)
		{
			return Pawn->ActorHasTag(Tag);
		}
	}
	return false;
}

