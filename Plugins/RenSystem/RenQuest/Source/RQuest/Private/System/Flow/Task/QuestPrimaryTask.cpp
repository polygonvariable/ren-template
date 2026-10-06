// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/Flow/Task/QuestPrimaryTask.h"

// Engine Headers
#include "UObject/ObjectSaveContext.h"

//
#include "Actor/QuestObjectiveActor.h"
#include "Core/QuestSettings.h"
#include "Core/QuestSettings.h"
#include "Data/QuestAsset.h"
#include "Log/LogMacro.h"
#include "LuauLibrary.h"
#include "LuauProperty.h"
#include "LuauProperty.h"
#include "LuauSubsystem.h"
#include "PropertyBagLibrary.h"
#include "Task/EventflowSubTask.h"
#include "Task/EventflowSubTask.h"
#include "Type/EventflowGraphData.h"



UQuestPrimaryTask::UQuestPrimaryTask()
{
	bAllowSubTasks = true;
}








void UQuestTask_ExternalTask::PreSave(FObjectPreSaveContext ObjectSaveContext)
{
#if WITH_EDITOR
	ExternalPinIds.Empty();
	ExternalPinTitles.Empty();

	if (ExternalTask)
	{
		const TMap<FGuid, FEventflowNode>& Nodes = ExternalTask->NodeCollection;
		for (const TPair<FGuid, FEventflowNode>& Kv : Nodes)
		{
			UEventflowPrimaryTask* NTask = Kv.Value.Task;
			if (IsValid(NTask) && NTask->TaskType == EEventflowPrimaryTaskType::Exit)
			{
				ExternalPinIds.Add(Kv.Key);
				ExternalPinTitles.Add(NTask->TaskTitle);
			}
		}
	}
#endif

	Super::PreSave(ObjectSaveContext);
}








UQuestTask_Reroute::UQuestTask_Reroute()
{

}

TInstancedStruct<FEventflowTransitionData>& UQuestTask_Reroute::GetTransitionData(EFSMResult Result)
{
	TInstancedStruct<FEventflowTransitionData>& TransitionData = Super::GetTransitionData(Result);
	FEventflowTransitionData& Data = TransitionData.GetMutable();
	Data.NextNodeId = RerouteId;
	if (RerouteType == ERerouteType::Target)
	{
		Data.Type = EEventflowTransitionType::NextNode;
	}
	else
	{
		Data.Type = EEventflowTransitionType::RedirectNode;
	}
	return TransitionData;
}

void UQuestTask_Reroute::CopyFromAsset(const UEventflowTask* Template)
{
	Super::CopyFromAsset(Template);

	const UQuestTask_Reroute* Task = Cast<UQuestTask_Reroute>(Template);
	if (IsValid(Task))
	{
		RerouteType = Task->RerouteType;
		RerouteName = Task->RerouteName;
		RerouteId = Task->RerouteId;
	}
}

void UQuestTask_Reroute::PreSave(FObjectPreSaveContext ObjectSaveContext)
{
#if WITH_EDITOR
	if (RerouteType == ERerouteType::Source)
	{
		RerouteId.Invalidate();

		const UQuestAsset* Asset = Cast<UQuestAsset>(GetOuter());
		if (IsValid(Asset))
		{
			const TMap<FGuid, FEventflowNode>& Nodes = Asset->NodeCollection;
			for (const TPair<FGuid, FEventflowNode>& Kv : Nodes)
			{
				const UQuestTask_Reroute* Reroute = Cast<UQuestTask_Reroute>(Kv.Value.Task);
				if (Reroute && Reroute->RerouteName.Equals(RerouteName) && Reroute->RerouteType == ERerouteType::Target)
				{
					RerouteId = Kv.Key;
				}
			}
		}
	}
#endif

	Super::PreSave(ObjectSaveContext);
}

void UQuestTask_Reroute::OnInitialized(EFSMState PreviousState)
{
	Super::OnInitialized(PreviousState);
	Load();
}

void UQuestTask_Reroute::OnLoaded(EFSMState PreviousState)
{
	Ready();
}

void UQuestTask_Reroute::OnReady(EFSMState PreviousState)
{
	Execute();
}

void UQuestTask_Reroute::OnActive(EFSMState PreviousState)
{
	Finish(EFSMResult::Success);
}





UQuestTask_Begin::UQuestTask_Begin()
{
	TaskType = EEventflowPrimaryTaskType::Entry;
}

void UQuestTask_Begin::OnInitialized(EFSMState PreviousState)
{
	Super::OnInitialized(PreviousState);
	Load();
}

void UQuestTask_Begin::OnLoaded(EFSMState PreviousState)
{
	Ready();
}

void UQuestTask_Begin::OnReady(EFSMState PreviousState)
{
	Execute();
}

void UQuestTask_Begin::OnActive(EFSMState PreviousState)
{
	Finish(EFSMResult::Success);
}


UQuestTask_End::UQuestTask_End()
{
	TaskType = EEventflowPrimaryTaskType::Exit;
}

void UQuestTask_End::OnInitialized(EFSMState PreviousState)
{
	Super::OnInitialized(PreviousState);
	Load();
}

void UQuestTask_End::OnLoaded(EFSMState PreviousState)
{
	Ready();
}

void UQuestTask_End::OnReady(EFSMState PreviousState)
{
	Execute();
}

void UQuestTask_End::OnActive(EFSMState PreviousState)
{
	Finish(EFSMResult::Success);
}























UQuestTask_SpawnMarker::UQuestTask_SpawnMarker()
{
	bAllowSubTasks = false;
}

void UQuestTask_SpawnMarker::CopyFromAsset(const UEventflowTask* Template)
{
	Super::CopyFromAsset(Template);
	
	const UQuestTask_SpawnMarker* Task = Cast<UQuestTask_SpawnMarker>(Template);
	if (IsValid(Task))
	{
		MarkerClass = Task->MarkerClass;
		MarkerTransform = Task->MarkerTransform;
	}
}

#if WITH_EDITOR
void UQuestTask_SpawnMarker::AppendAssetBundleData(FAssetBundleData& AssetBundle)
{
	Super::AppendAssetBundleData(AssetBundle);

	const UQuestSettings* Settings = UQuestSettings::Get();
	const FName& BundleName = Settings->BundleName;

	AssetBundle.AddBundleAsset(BundleName, MarkerClass.ToSoftObjectPath().GetAssetPath());
}
#endif

void UQuestTask_SpawnMarker::OnInitialized(EFSMState PreviousStatus)
{
	Super::OnInitialized(PreviousStatus);
	Load();
}

void UQuestTask_SpawnMarker::OnLoaded(EFSMState PreviousStatus)
{
	MarkerActor = GetWorld()->SpawnActorDeferred<AQuestObjectiveMarker>(MarkerClass.Get(), MarkerTransform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	if (IsValid(MarkerActor))
	{
		MarkerActor->OnInteractionCompleted.BindUObject(this, &UQuestTask_SpawnMarker::HandleOnInteractionCompleted);
		MarkerActor->FinishSpawning(MarkerTransform);
		Ready();
	}
}

void UQuestTask_SpawnMarker::OnReady(EFSMState PreviousState)
{
	if (IsValid(MarkerActor))
	{
		MarkerActor->SetActorHiddenInGame(false);
		MarkerActor->SetActorEnableCollision(true);
	}
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

void UQuestTask_SpawnMarker::HandleOnInteractionCompleted()
{
	Active();
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

void UQuestTask_ConditionalSpawnMarker::HandleOnInteractionCompleted()
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































void UQuestTask_Countdown::CopyFromAsset(const UEventflowTask* Template)
{
	Super::CopyFromAsset(Template);
	const UQuestTask_Countdown* T = Cast<UQuestTask_Countdown>(Template);
	if(IsValid(T))
	{
		Duration = T->Duration;
	}
}

#if UE_BUILD_DEVELOPMENT
void UQuestTask_Countdown::GetEditorDebugInfo(TArray<FString>& OutDebug) const
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	OutDebug.Push(TEXT("Time:\t") + FString::FromInt(TimerManager.GetTimerElapsed(TimerHandle)));
}
#endif

void UQuestTask_Countdown::HandleCountdownOver()
{
	Finish(EFSMResult::Success);
}

void UQuestTask_Countdown::OnInitialized(EFSMState PreviousState)
{
	Super::OnInitialized(PreviousState);

	Load();
}

void UQuestTask_Countdown::OnLoaded(EFSMState PreviousState)
{
	Ready();
}

void UQuestTask_Countdown::OnReady(EFSMState PreviousState)
{

}

void UQuestTask_Countdown::OnActive(EFSMState PreviousState)
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.SetTimer(TimerHandle, this, &UQuestTask_Countdown::HandleCountdownOver, Duration);
}

void UQuestTask_Countdown::OnEndActive(EFSMState NextState, EFSMResult Result)
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.ClearTimer(TimerHandle);
	TimerHandle.Invalidate();
}

void UQuestTask_Countdown::OnRestart(EFSMState PreviousState, EFSMResult PreviousResult)
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.ClearTimer(TimerHandle);
	TimerHandle.Invalidate();
}

void UQuestTask_Countdown::OnReset()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.ClearTimer(TimerHandle);
	TimerHandle.Invalidate();

	Super::OnReset();
}








#if WITH_EDITOR
void UQuestTask_ReachLocation::AppendAssetBundleData(FAssetBundleData& AssetBundle)
{
	Super::AppendAssetBundleData(AssetBundle);

	const UQuestSettings* Settings = UQuestSettings::Get();
	const FName& BundleName = Settings->BundleName;

	const TSoftClassPtr<AQuestObjectiveActor>& ActorClass = ReachArea.ActorClass;

	AssetBundle.AddBundleAsset(BundleName, ActorClass.ToSoftObjectPath().GetAssetPath());
}
#endif

void UQuestTask_ReachLocation::CopyFromAsset(const UEventflowTask* Template)
{
	Super::CopyFromAsset(Template);
	const UQuestTask_ReachLocation* T = Cast<UQuestTask_ReachLocation>(Template);
	if (IsValid(T))
	{
		ReachArea = T->ReachArea;
	}
}

void UQuestTask_ReachLocation::HandleOnDestinationReached(EFSMResult Result)
{
	Finish(EFSMResult::Success);
}

void UQuestTask_ReachLocation::OnInitialized(EFSMState PreviousState)
{
	Super::OnInitialized(PreviousState);
	Load();
}

void UQuestTask_ReachLocation::OnLoaded(EFSMState PreviousState)
{
	UClass* ActorClass = ReachArea.ActorClass.Get();
	UWorld* World = GetWorld();

	RuntimeActor = World->SpawnActorDeferred<AQuestObjectiveActor>(ActorClass, ReachArea.Transform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	if (IsValid(RuntimeActor))
	{
		FPropertyBagLibrary::CopyPropertiesToInstance(ReachArea.Properties, RuntimeActor);

		RuntimeActor->OnCompleted.BindUObject(this, &UQuestTask_ReachLocation::HandleOnDestinationReached);
		RuntimeActor->SetOwningTask(this);
		RuntimeActor->FinishSpawning(ReachArea.Transform);
	}

	Ready();
}

void UQuestTask_ReachLocation::OnReady(EFSMState PreviousState)
{
	if (IsValid(RuntimeActor))
	{
		RuntimeActor->SetActorHiddenInGame(true);
		RuntimeActor->SetActorEnableCollision(false);
	}
}

void UQuestTask_ReachLocation::OnActive(EFSMState PreviousState)
{
	if (IsValid(RuntimeActor))
	{
		RuntimeActor->SetActorHiddenInGame(false);
		RuntimeActor->SetActorEnableCollision(true);
	}
}

void UQuestTask_ReachLocation::OnEndActive(EFSMState NextState, EFSMResult Result)
{
	if (IsValid(RuntimeActor))
	{
		RuntimeActor->SetActorHiddenInGame(true);
		RuntimeActor->SetActorEnableCollision(false);
	}
}

void UQuestTask_ReachLocation::OnFinished(EFSMResult Result)
{
}

void UQuestTask_ReachLocation::OnRestart(EFSMState PreviousState, EFSMResult PreviousResult)
{

}

void UQuestTask_ReachLocation::OnReset()
{
	if (IsValid(RuntimeActor))
	{
		RuntimeActor->OnCompleted.Unbind();
		RuntimeActor->SetOwningTask(nullptr);
		RuntimeActor->Destroy();
	}
	RuntimeActor = nullptr;
	
	Super::OnReset();
}






void UQuestTask_ActorHaveTag::CopyFromAsset(const UEventflowTask* Template)
{
	Super::CopyFromAsset(Template);
	const UQuestTask_ActorHaveTag* T = Cast<UQuestTask_ActorHaveTag>(Template);
	if (IsValid(T))
	{
		Tag = T->Tag;
	}
}

void UQuestTask_ActorHaveTag::OnInitialized(EFSMState PreviousState)
{
	Load();
}

void UQuestTask_ActorHaveTag::OnLoaded(EFSMState PreviousState)
{
	Ready();
}

void UQuestTask_ActorHaveTag::OnReady(EFSMState PreviousState)
{

}

void UQuestTask_ActorHaveTag::OnActive(EFSMState PreviousState)
{
	UWorld* World = GetWorld();
	APlayerController* PC = World->GetFirstPlayerController();
	if (PC)
	{
		APawn* Pawn = PC->GetPawn();
		if (Pawn)
		{
			Finish(Pawn->ActorHasTag(Tag) ? EFSMResult::Success : EFSMResult::Failed);
			return;
		}
	}
	Finish(EFSMResult::Failed);
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










