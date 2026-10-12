// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/Flow/Task/QuestSubTask_Countdown.h"

// Project Headers
#include "Core/QuestEvents.h"


void UQuestTask_Countdown::PullQuestData(TInstancedStruct<FQuestRuntimeData>& OutData)
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	FQuestRuntimeData& Data = OutData.InitializeAs<FQuestRuntimeData>();
	Data.Value = FString::FromInt(TimerManager.GetTimerElapsed(TimerHandle));
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

void UQuestTask_Countdown::HandleOnTick()
{
	FQuestEvents::DataChanged(GetWorld());
}

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
	FQuestEvents::DataChanged(GetWorld());

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.SetTimer(TimerHandle, this, &UQuestTask_Countdown::HandleCountdownOver, Duration);
	TimerManager.SetTimer(TickHandle, this, &UQuestTask_Countdown::HandleOnTick, 1.0f, FTimerManagerTimerParameters{ .bLoop = true, .bMaxOncePerFrame = true });
}

void UQuestTask_Countdown::OnEndActive(EFSMState NextState, EFSMResult Result)
{
	FQuestEvents::DataChanged(GetWorld());

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.ClearTimer(TimerHandle);
	TimerHandle.Invalidate();
	TimerManager.ClearTimer(TickHandle);
	TickHandle.Invalidate();
}

void UQuestTask_Countdown::OnRestart(EFSMState PreviousState, EFSMResult PreviousResult)
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.ClearTimer(TimerHandle);
	TimerHandle.Invalidate();
	TimerManager.ClearTimer(TickHandle);
	TickHandle.Invalidate();
}

void UQuestTask_Countdown::OnReset()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.ClearTimer(TimerHandle);
	TimerHandle.Invalidate();
	TimerManager.ClearTimer(TickHandle);
	TickHandle.Invalidate();

	Super::OnReset();
}

