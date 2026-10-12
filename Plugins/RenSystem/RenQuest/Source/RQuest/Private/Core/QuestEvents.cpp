// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Core/QuestEvents.h"

// Project Headers
#include "System/QuestSubsystem.h"


void FQuestEvents::DataChanged(UWorld* World)
{
	UQuestSubsystem* Subsystem = UQuestSubsystem::Get(World);
	if (!IsValid(Subsystem))
	{
		ensureMsgf(false, TEXT("Unable to fetch quest subsystem to broadcasr delegate"));
		return;
	}
	Subsystem->OnQuestDataChanged.Broadcast();
}
