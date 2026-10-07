// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/Flow/Task/QuestPrimaryTask.h"

// Engine Headers
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif
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
#include "ComponentTemplateLibrary.h"
#include "EventflowLibrary.h"
#include "EventflowEngineProvider.h"
#include "EventflowEngine.h"
#include "MiscLibrary.h"


UQuestPrimaryTask::UQuestPrimaryTask()
{
	bAllowSubTasks = false;
}















TInstancedStruct<FEventflowNodeTransitionData>& UQuestTask_Reroute::GetTransitionData(EFSMResult Result)
{
	TInstancedStruct<FEventflowNodeTransitionData>& TransitionData = Super::GetTransitionData(Result);
	FEventflowNodeTransitionData* Data = TransitionData.GetMutablePtr();
	checkf(Data, TEXT("Node transition data is invalid"));

	Data->NextNodeId = RerouteId;
	if (RerouteType == ERerouteType::Target)
	{
		Data->NodeTransition = EEventflowNodeTransitionType::NextNode;
	}
	else
	{
		Data->NodeTransition = EEventflowNodeTransitionType::RedirectNode;
	}
	return TransitionData;
}

void UQuestTask_Reroute::CopyFromAsset(const UEventflowTask* Template)
{
	Super::CopyFromAsset(Template);

	const UQuestTask_Reroute* Task = Cast<UQuestTask_Reroute>(Template);
	checkf(Task, TEXT("Invalid task template"));

	RerouteType = Task->RerouteType;
	RerouteName = Task->RerouteName;
	RerouteId = Task->RerouteId;
}

void UQuestTask_Reroute::PreSave(FObjectPreSaveContext ObjectSaveContext)
{
#if WITH_EDITOR
	RerouteId.Invalidate();
	
	if (RerouteType == ERerouteType::Source)
	{
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
	Active();
}

void UQuestTask_Reroute::OnActive(EFSMState PreviousState)
{
	Finish(EFSMResult::Success);
}





UQuestTask_Begin::UQuestTask_Begin()
{
	NodeType = EEventflowNodeType::Entry;
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
	Active();
}

void UQuestTask_Begin::OnActive(EFSMState PreviousState)
{
	Finish(EFSMResult::Success);
}


UQuestTask_End::UQuestTask_End()
{
	NodeType = EEventflowNodeType::Exit;
}

void UQuestTask_End::GetReturnData(TInstancedStruct<FEventflowReturnData>& ReturnData)
{
	Super::GetReturnData(ReturnData);

	FEventflowReturnData* Data = ReturnData.GetMutablePtr();
	checkf(Data, TEXT("Node return data is invalid"));

	Data->GraphTransition = GraphResult;
}

void UQuestTask_End::CopyFromAsset(const UEventflowTask* Template)
{
	Super::CopyFromAsset(Template);

	const UQuestTask_End* Task = Cast<UQuestTask_End>(Template);
	checkf(IsValid(Task), TEXT("Invalid task template"));

	GraphResult = Task->GraphResult;
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
	Active();
}

void UQuestTask_End::OnActive(EFSMState PreviousState)
{
	Finish(EFSMResult::Success);
}























UQuestTask_SpawnMarker::UQuestTask_SpawnMarker()
{
}

void UQuestTask_SpawnMarker::CopyFromAsset(const UEventflowTask* Template)
{
	Super::CopyFromAsset(Template);
	
	const UQuestTask_SpawnMarker* Task = Cast<UQuestTask_SpawnMarker>(Template);
	checkf(Task, TEXT("Invalid task template"));

	MarkerClass = Task->MarkerClass;
	MarkerTransform = Task->MarkerTransform;
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

void UQuestTask_SpawnMarker::OnInitialized(EFSMState PreviousState)
{
	Super::OnInitialized(PreviousState);
	FMiscLibrary::NextFrame(this, &UQuestTask_SpawnMarker::Load);
}

void UQuestTask_SpawnMarker::OnLoaded(EFSMState PreviousState)
{
	UQuestTask_SpawnMarker* Template = GetOwningTemplate<UQuestTask_SpawnMarker>();
	checkf(Template, TEXT("Invalid task template"));

	MarkerActor = GetWorld()->SpawnActorDeferred<AQuestObjectiveMarker>(MarkerClass.Get(), MarkerTransform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	if (IsValid(MarkerActor))
	{
		MarkerActor->OnInteractionCompleted.BindUObject(this, &UQuestTask_SpawnMarker::HandleOnInteractionCompleted);
		MarkerActor->FinishSpawning(MarkerTransform);
		FComponentTemplateLibrary::RegisterComponents(MarkerActor, Template->MarkerComponents);
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


void UQuestTask_ExternalTask::CopyFromAsset(const UEventflowTask* Template)
{
	Super::CopyFromAsset(Template);

	const UQuestTask_ExternalTask* Task = Cast<UQuestTask_ExternalTask>(Template);
	checkf(Task, TEXT("Invalid task template"));

	ExternalAsset = Task->ExternalAsset;
	ExternalPins = Task->ExternalPins;
}

#if WITH_EDITOR
void UQuestTask_ExternalTask::AppendAssetBundleData(FAssetBundleData& AssetBundle)
{
	Super::AppendAssetBundleData(AssetBundle);

	const UQuestSettings* Settings = UQuestSettings::Get();
	const FName& BundleName = Settings->BundleName;

	if (!ExternalAsset.IsNull())
	{
		AssetBundle.AddBundleAsset(BundleName, ExternalAsset.ToSoftObjectPath().GetAssetPath());
	}
}
EDataValidationResult UQuestTask_ExternalTask::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	if (ExternalAsset.IsNull())
	{
		Context.AddError(FText::FromString("External eventflow asset is invalid"));
		return EDataValidationResult::Invalid;
	}

	if (ExternalPins.IsEmpty())
	{
		Context.AddError(FText::FromString("External evetflow have invalid exit pins"));
		return EDataValidationResult::Invalid;
	}

	return Result;
}
#endif

void UQuestTask_ExternalTask::HandleOnEngineRemoved(FPrimaryAssetId AssetId, UEventflowEngine* Engine)
{
	const UEventflowAsset* TaskAsset = ExternalAsset.Get();
	checkf(IsValid(TaskAsset), TEXT("Failed to get external task asset"));

	FPrimaryAssetId TaskAssetId = TaskAsset->GetPrimaryAssetId();
	if (TaskAssetId != AssetId)
	{
		return;
	}

	IEventflowEngineProvider* EngineProvider = FEventflowLibrary::GetEngineProvider(GetWorld(), TaskAssetId);
	checkf(EngineProvider, TEXT("Failed to get external task engine provider"));

	EngineProvider->GetOnEngineRemoved().RemoveAll(this);

	const TInstancedStruct<FEventflowReturnData>& ReturnData = Engine->GetReturnData();
	if (!ReturnData.IsValid())
	{
		return;
	}

	const FEventflowReturnData& Data = ReturnData.Get();
	FGuid ExitId = Data.ExitNodeId;

	int Index = ExternalPins.IndexOfByPredicate([ExitId](const FGuid& NodeId) { return NodeId == ExitId; });
	if (Index >= 0)
	{
		ModifyTransitionData(
			[Index](TInstancedStruct<FEventflowNodeTransitionData>& TransitionData)
			{
				FEventflowNodeTransitionData& Data = TransitionData.GetMutable();
				Data.NextNodeIndex = Index;
			}
		);

		TWeakObjectPtr<UQuestTask_ExternalTask> WeakThis(this);

		FTimerManager& TimerManager = GetWorld()->GetTimerManager();
		TimerManager.SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this,
			[WeakThis]()
			{
				UQuestTask_ExternalTask* This = WeakThis.Get();
				checkf(This, TEXT("Failed to resolve task pointer"));

				This->Finish(EFSMResult::Success);
			}
		));
	}
}

void UQuestTask_ExternalTask::OnInitialized(EFSMState PreviousState)
{
	Super::OnInitialized(PreviousState);
	Load();
}

void UQuestTask_ExternalTask::OnLoaded(EFSMState PreviousState)
{
	Ready();
}

void UQuestTask_ExternalTask::OnReady(EFSMState PreviousState)
{
	Active();
}

void UQuestTask_ExternalTask::OnActive(EFSMState PreviousState)
{
	const UEventflowAsset* TaskAsset = ExternalAsset.Get();
	checkf(IsValid(TaskAsset), TEXT("Failed to get external task asset"));

	FPrimaryAssetId TaskAssetId = TaskAsset->GetPrimaryAssetId();
	IEventflowEngineProvider* EngineProvider = FEventflowLibrary::GetEngineProvider(GetWorld(), TaskAssetId);
	checkf(EngineProvider, TEXT("Failed to get external task engine provider"));

	EngineProvider->GetOnEngineRemoved().AddUObject(this, &UQuestTask_ExternalTask::HandleOnEngineRemoved);
	EngineProvider->StartEventflow(TaskAssetId);
}

void UQuestTask_ExternalTask::OnReset()
{
	const UEventflowAsset* TaskAsset = ExternalAsset.Get();
	if (IsValid(TaskAsset))
	{
		FPrimaryAssetId TaskAssetId = TaskAsset->GetPrimaryAssetId();
		IEventflowEngineProvider* EngineProvider = FEventflowLibrary::GetEngineProvider(GetWorld(), TaskAssetId);
		if (EngineProvider)
		{
			EngineProvider->GetOnEngineRemoved().RemoveAll(this);
		}
	}

	Super::OnReset();
}










UQuestTask_SubtaskGate::UQuestTask_SubtaskGate()
{
	bAllowSubTasks = true;
}

void UQuestTask_SubtaskGate::PreSave(FObjectPreSaveContext ObjectSaveContext)
{
#if WITH_EDITOR
	if (bAutoCompile)
	{
		if (!FLuauLibrary::Compile(LuauCode))
		{
			LuauCode.Bytecode.Empty();
		}
	}
#endif

	Super::PreSave(ObjectSaveContext);
}

void UQuestTask_SubtaskGate::HandleOnSubTaskStateChanged(EFSMState PreviousState, EFSMState NewState, EFSMResult Result)
{
	if (NewState != EFSMState::Finished)
	{
		return;
	}

	const UQuestTask_SubtaskGate* Template = GetOwningTemplate<UQuestTask_SubtaskGate>();
	checkf(IsValid(Template), TEXT("Invalid template task"));

	ULuauSubsystem* LuauSubsystem = ULuauSubsystem::Get(GetWorld());
	if (!IsValid(Template) || !IsValid(LuauSubsystem))
	{
		return;
	}

	FLuauProperties Inputs;
	FLuauProperties Outputs;

	FLuauProperty_Table& TaskTable = Inputs.SetTable();
	TaskTable.Set(TEXT("state"), static_cast<int>(GetState()));
	TaskTable.Set(TEXT("result"), static_cast<int>(GetResult()));

	FLuauProperty_Table& SubTaskTable = Inputs.SetTable();
	const TArray<UEventflowSubTask*>& Tasks = GetSubTasks();
	for (UEventflowSubTask* Task : Tasks)
	{
		if (IsValid(Task))
		{
			FLuauProperty_Table& SubTaskTableEntry = SubTaskTable.SetTable(Task->TaskName.ToString());
			SubTaskTableEntry.Set(TEXT("state"), static_cast<int>(Task->GetState()));
			SubTaskTableEntry.Set(TEXT("result"), static_cast<int>(Task->GetResult()));
		}
	}

	bool bSuccess = LuauSubsystem->ExecuteBytecode(Template->LuauCode.Bytecode, TEXT("script"), TEXT("evaluateSubTasks"), Inputs, Outputs);
	if (!ensureMsgf(bSuccess, TEXT("Failed to execute quest luau bytecode")))
	{
		return;
	}

	const FLuauProperty_Number* OutputResult = Outputs.Get<FLuauProperty_Number>(0);
	if (!ensureMsgf(OutputResult, TEXT("Failed to get output from luau method")))
	{
		return;
	}

	int Index = 2;
	EFSMResult FSMResult = static_cast<EFSMResult>(OutputResult->Value);
	switch (FSMResult)
	{
	case EFSMResult::Success:
		Index = 0;
		break;
	case EFSMResult::Failed:
		Index = 1;
		break;
	case EFSMResult::Cancelled:
		Index = 2;
		break;
	}

	ModifyTransitionData(
		[Index](TInstancedStruct<FEventflowNodeTransitionData>& TransitionData)
		{
			FEventflowNodeTransitionData& Data = TransitionData.GetMutable();
			Data.NextNodeIndex = Index;
		}
	);

	Finish(FSMResult);
}

void UQuestTask_SubtaskGate::OnInitialized(EFSMState PreviousState)
{
	Super::OnInitialized(PreviousState);
	Load();
}

void UQuestTask_SubtaskGate::OnLoaded(EFSMState PreviousState)
{
	Ready();
}

void UQuestTask_SubtaskGate::OnReady(EFSMState PreviousState)
{
	Active();
}

void UQuestTask_SubtaskGate::OnActive(EFSMState PreviousState)
{
	const TArray<UEventflowSubTask*>& Tasks = GetSubTasks();
	for (UEventflowSubTask* Task : Tasks)
	{
		if (IsValid(Task))
		{
			Task->Active();
		}
	}
}

void UQuestTask_SubtaskGate::OnFinished(EFSMResult Result)
{
}

void UQuestTask_SubtaskGate::OnReset()
{
	Super::OnReset();
}





























UQuestTask_SpawnLocation::UQuestTask_SpawnLocation()
{
}

void UQuestTask_SpawnLocation::CopyFromAsset(const UEventflowTask* Template)
{
	Super::CopyFromAsset(Template);

	const UQuestTask_SpawnLocation* Task = Cast<UQuestTask_SpawnLocation>(Template);
	checkf(Task, TEXT("Invalid task template"));

	MarkerActor = Task->MarkerActor;
	MarkerClass = Task->MarkerClass;
	MarkerTransform = Task->MarkerTransform;
}

#if WITH_EDITOR
void UQuestTask_SpawnLocation::AppendAssetBundleData(FAssetBundleData& AssetBundle)
{
	Super::AppendAssetBundleData(AssetBundle);

	const UQuestSettings* Settings = UQuestSettings::Get();
	const FName& BundleName = Settings->BundleName;

	AssetBundle.AddBundleAsset(BundleName, MarkerClass.ToSoftObjectPath().GetAssetPath());
}
#endif

void UQuestTask_SpawnLocation::OnInitialized(EFSMState PreviousState)
{
	Super::OnInitialized(PreviousState);
	FMiscLibrary::NextFrame(this, &UQuestTask_SpawnLocation::Load);
}

void UQuestTask_SpawnLocation::OnLoaded(EFSMState PreviousState)
{
	MarkerActor = GetWorld()->SpawnActorDeferred<AQuestObjectiveMarker>(MarkerClass.Get(), MarkerTransform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	if (IsValid(MarkerActor))
	{
		MarkerActor->OnInteractionCompleted.BindUObject(this, &UQuestTask_SpawnLocation::HandleOnInteractionCompleted);
		MarkerActor->FinishSpawning(MarkerTransform);
		Ready();
	}
}

void UQuestTask_SpawnLocation::OnReady(EFSMState PreviousState)
{
	if (IsValid(MarkerActor))
	{
		MarkerActor->SetActorHiddenInGame(false);
		MarkerActor->SetActorEnableCollision(true);
	}
}

void UQuestTask_SpawnLocation::OnActive(EFSMState PreviousState)
{
	Finish(EFSMResult::Success);
}

void UQuestTask_SpawnLocation::OnFinished(EFSMResult Result)
{
	if (IsValid(MarkerActor))
	{
		MarkerActor->SetActorHiddenInGame(true);
		MarkerActor->SetActorEnableCollision(false);
	}
}

void UQuestTask_SpawnLocation::OnReset()
{
	if (IsValid(MarkerActor))
	{
		MarkerActor->Destroy();
	}
	MarkerActor = nullptr;

	Super::OnReset();
}

void UQuestTask_SpawnLocation::HandleOnInteractionCompleted()
{
	Active();
}

