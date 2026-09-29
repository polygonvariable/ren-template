// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "GameplayModeSubsystem.h"

// Engine Headers
#include "Engine/AssetManager.h"

// Project Headers
#include "Core/AssetManagerUtil.h"
#include "GameplayModeSettings.h"
#include "GameplayModeTagGroup.h"
#include "GameplayModeWorldConfig.h"
#include "Log/LogMacro.h"
#include "WorldFragmentSettings.h"


void UGameplayModeSubsystem::BP_PushGameplayMode(FName Mode)
{
	PushGameplayMode(Mode);
}

void UGameplayModeSubsystem::BP_PopGameplayMode(FName Mode)
{
	PopGameplayMode(Mode);
}


void UGameplayModeSubsystem::PushGameplayMode(FName Mode)
{
	if (!IsValid(GameplayModeTable))
	{
		return;
	}

	const FGameplayModeTagGroup* FoundRow = GameplayModeTable->FindRow<FGameplayModeTagGroup>(Mode, FString());
	if (!FoundRow)
	{
		return;
	}

	GameplayModeStack.Push(Mode);

	AddGameplayMode(FoundRow->ActivateTags);
	RempoveGameplayMode(FoundRow->DeactivateTags);
}

void UGameplayModeSubsystem::PopGameplayMode(FName Mode)
{
	if (!IsValid(GameplayModeTable) || GameplayModeStack.IsEmpty())
	{
		return;
	}

	FName LastMode = GameplayModeStack.Pop();
	const FGameplayModeTagGroup* LastRow = GameplayModeTable->FindRow<FGameplayModeTagGroup>(LastMode, FString());
	if (!LastRow)
	{
		return;
	}

	FGameplayTagContainer NewActiveTags;
	FGameplayTagContainer LastActiveTags = LastRow->ActivateTags;

	if (!GameplayModeStack.IsEmpty())
	{
		FName NewLast = GameplayModeStack.Last();
		const FGameplayModeTagGroup* NewLastRow = GameplayModeTable->FindRow<FGameplayModeTagGroup>(NewLast, FString());
		if (NewLastRow)
		{
			LastActiveTags.AppendTags(NewLastRow->DeactivateTags);
			NewActiveTags.AppendTags(NewLastRow->ActivateTags);
		}
	}

	AddGameplayMode(NewActiveTags);
	RempoveGameplayMode(LastActiveTags);
}

void UGameplayModeSubsystem::RegisterTagNotify(FGameplayTag Tag, FOnGameplayModeTagChanged::FDelegate&& Callback)
{
	if (!Tag.IsValid())
	{
		return;
	}

	TPair<FOnGameplayModeTagChanged, int>& Pair = Handles.FindOrAdd(Tag);
	Pair.Key.Add(Callback);
	Pair.Value++;

	LOG_WARNING(LogTemp, TEXT("GameplayMode tag callback added: %s"), *Tag.ToString());
}

void UGameplayModeSubsystem::UnregisterTagNotify(FGameplayTag Tag, UObject* Target)
{
	if (!Tag.IsValid())
	{
		return;
	}

	TPair<FOnGameplayModeTagChanged, int>* Pair = Handles.Find(Tag);
	if (Pair)
	{
		Pair->Key.RemoveAll(Target);
		Pair->Value--;

		if (Pair->Value == 0)
		{
			Pair->Key.Clear();
			Handles.Remove(Tag);

			LOG_WARNING(LogTemp, TEXT("GameplayMode tag callback cleared"));
		}
	}

	LOG_WARNING(LogTemp, TEXT("GameplayMode tag callback removed: %s"), *Tag.ToString());
}

const FGameplayTagContainer& UGameplayModeSubsystem::GetGameplayModeTags() const
{
	return GameplayModeTag;
}

void UGameplayModeSubsystem::AddGameplayMode(FGameplayTagContainer Tags)
{
	for (const FGameplayTag& Tag : Tags)
	{
		if (!Tag.IsValid() || GameplayModeTag.HasTagExact(Tag))
		{
			continue;
		}
		GameplayModeTag.AddTag(Tag);
		BroadcastTagChange(Tag, true);
	}
}

void UGameplayModeSubsystem::RempoveGameplayMode(FGameplayTagContainer Tags)
{
	for (const FGameplayTag& Tag : Tags)
	{
		if (!Tag.IsValid())
		{
			continue;
		}
		if (GameplayModeTag.RemoveTag(Tag))
		{
			BroadcastTagChange(Tag, false);
		}
	}
}


void UGameplayModeSubsystem::BroadcastTagChange(FGameplayTag Tag, bool bAdded)
{
	if (bCanBroadcast)
	{
		const TPair<FOnGameplayModeTagChanged, int>* FoundHandle = Handles.Find(Tag);
		if (FoundHandle)
		{
			FoundHandle->Key.Broadcast(bAdded);
		}
		OnGameplayModeTagsChanged.Broadcast(Tag, bAdded);
	}
}

void UGameplayModeSubsystem::HandleOnGameplayModeTableLoaded()
{
	const UGameplayModeSettings* Settings = UGameplayModeSettings::Get();
	GameplayModeTable = Settings->GameplayModeTable.Get();

	if (!IsValid(GameplayModeTable))
	{
		return;
	}

	const UGameplayModeWorldConfig* ModeConfig = AWorldFragmentSettings::GetConfigByClass<UGameplayModeWorldConfig>(GetWorld());
	checkf(IsValid(ModeConfig) && ModeConfig->bEnabled, TEXT("GameplayMode world config is invalid or disabled"));

	const FGameplayModeTagGroup* FoundRow = GameplayModeTable->FindRow<FGameplayModeTagGroup>(ModeConfig->DefaultMode, FString());
	if (!FoundRow)
	{
		return;
	}

	GameplayModeStack.Push(ModeConfig->DefaultMode);

	if (HasCalledBeginPlay())
	{
		bCanBroadcast = true;
	}

	AddGameplayMode(FoundRow->ActivateTags);
	RempoveGameplayMode(FoundRow->DeactivateTags);
}


bool UGameplayModeSubsystem::DoesSupportWorldType(EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

bool UGameplayModeSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!Super::ShouldCreateSubsystem(Outer))
	{
		return false;
	}
	const UGameplayModeWorldConfig* Config = AWorldFragmentSettings::GetConfigByClass<UGameplayModeWorldConfig>(Cast<UWorld>(Outer));
	return IsValid(Config) && Config->bEnabled;
}

void UGameplayModeSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogTemp, TEXT("GameplayModeSubsystem Initialized"));
	
	const UGameplayModeSettings* Settings = UGameplayModeSettings::Get();

	FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
	TableHandle = StreamableManager.RequestAsyncLoad(Settings->GameplayModeTable.ToSoftObjectPath(), FStreamableDelegate::CreateUObject(this, &UGameplayModeSubsystem::HandleOnGameplayModeTableLoaded));
}

void UGameplayModeSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	LOG_WARNING(LogTemp, TEXT("GameplayModeSubsystem OnWorldBeginPlay"));

	bCanBroadcast = true;
}

void UGameplayModeSubsystem::Deinitialize()
{
	FAssetManagerUtil::CancelHandle(TableHandle);
	bCanBroadcast = false;
	
	for (TPair<FGameplayTag, TPair<FOnGameplayModeTagChanged, int>>& Kv : Handles)
	{
		Kv.Value.Key.Clear();
	}
	Handles.Empty();

	LOG_WARNING(LogTemp, TEXT("GameplayModeSubsystem Deinitialized"));
	Super::Deinitialize();
}

UGameplayModeSubsystem* UGameplayModeSubsystem::Get(UWorld* World)
{
	if (!IsValid(World))
	{
		return nullptr;
	}
	return World->GetSubsystem<UGameplayModeSubsystem>();
}


#if UE_BUILD_DEVELOPMENT
const TArray<FName>& UGameplayModeSubsystem::GetEditorGameplayModeStack() const
{
	return GameplayModeStack;
}
#endif

