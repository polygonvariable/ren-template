// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Core/QuestSettings.h"

// Project Header
#include "Data/QuestStorage.h"
#include "System/Flow/QuestEngine.h"
#include "System/QuestStorageManager.h"
#include "System/QuestSubsystem.h"


UQuestSettings::UQuestSettings(const FObjectInitializer& ObjectInitializer)
{
	CategoryName = TEXT("Ren Project");

	StorageId = TEXT("quest001");
	StorageClass = UQuestStorage::StaticClass();
	StorageManagerClass = UQuestStorageManager::StaticClass();

	BundleName = TEXT("Quest");

	SubsystemClass = UQuestSubsystem::StaticClass();
	QuestEngineClass = UQuestEngine::StaticClass();
}

const UQuestSettings* UQuestSettings::Get()
{
	return GetDefault<UQuestSettings>();
}

