// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/GameplayModeSubsystem.h"

// Engine Headers
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

// Project Headers
#include "Data/GameplayModeAsset.h"
#include "Log/LogCategory.h"
#include "WorldFragmentSettings.h"
#include "Log/LogMacro.h"



const FGameplayTagContainer& UGameplayModeSubsystem::GetGameplayModeTags() const
{
	return GameplayModeTag;
}



void UGameplayModeSubsystem::RegisterTagNotify(FGameplayTag Tag, FOnGameplayModeTagChanged::FDelegate&& Callback)
{
	TPair<FOnGameplayModeTagChanged, int>& Pair = Handles.FindOrAdd(Tag);
	Pair.Key.Add(Callback);
	Pair.Value++;

	LOG_WARNING(LogTemp, TEXT("GameplayMode tag callback added: %s"), *Tag.ToString());
}

void UGameplayModeSubsystem::UnregisterTagNotify(FGameplayTag Tag, UObject* Target)
{
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



void UGameplayModeSubsystem::BP_SetGameplayMode(UGameplayModeAsset* Asset)
{
	SetGameplayMode(Asset);
}

void UGameplayModeSubsystem::BP_SetGameplayModeByTag(FGameplayTagContainer Tags)
{
	SetGameplayModeByTag(Tags);
}


void UGameplayModeSubsystem::BP_AddGameplayMode(FGameplayTagContainer Tags)
{
	AddGameplayMode(Tags);
}

void UGameplayModeSubsystem::BP_RempoveGameplayMode(FGameplayTagContainer Tags)
{
	RempoveGameplayMode(Tags);
}


void UGameplayModeSubsystem::SetGameplayMode(UPrimaryDataAsset* Asset)
{
	UGameplayModeAsset* ModeAsset = Cast<UGameplayModeAsset>(Asset);
	if (!IsValid(ModeAsset))
	{
		LOG_ERROR(LogTemp, TEXT("Gameplay mode asset is invalid"));
		return;
	}

	SetGameplayModeByTag(ModeAsset->GameplayModeTag);
}

void UGameplayModeSubsystem::SetGameplayModeByTag(FGameplayTagContainer Tags)
{
	FGameplayTagContainer NewTags = Tags;
	FGameplayTagContainer OldTags = GameplayModeTag;
	
	FGameplayTagContainer Removed = OldTags;
	Removed.RemoveTags(NewTags);

	FGameplayTagContainer Added = NewTags;
	Added.RemoveTags(OldTags);

	GameplayModeTag = NewTags;

	if (!bCanBroadcast)
	{
		LOG_WARNING(LogTemp, TEXT("GameplayModeSubsystem broadcast is disabled"));
		return;
	}

	for (const FGameplayTag& Tag : Removed)
	{
		const TPair<FOnGameplayModeTagChanged, int>* FoundHandle = Handles.Find(Tag);
		if (FoundHandle)
		{
			FoundHandle->Key.Broadcast(false);
		}
		OnGameplayModeTagsChanged.Broadcast(Tag, false);
	}

	for (const FGameplayTag& Tag : Added)
	{
		const TPair<FOnGameplayModeTagChanged, int>* FoundHandle = Handles.Find(Tag);
		if (FoundHandle)
		{
			FoundHandle->Key.Broadcast(true);
		}
		OnGameplayModeTagsChanged.Broadcast(Tag, true);
	}
}

void UGameplayModeSubsystem::AddGameplayMode(FGameplayTagContainer Tags)
{
	for (const FGameplayTag& Tag : Tags)
	{
		if (GameplayModeTag.HasTagExact(Tag))
		{
			continue;
		}

		GameplayModeTag.AddTag(Tag);

		if (bCanBroadcast)
		{
			const TPair<FOnGameplayModeTagChanged, int>* FoundHandle = Handles.Find(Tag);
			if (FoundHandle)
			{
				FoundHandle->Key.Broadcast(true);
			}
			OnGameplayModeTagsChanged.Broadcast(Tag, true);
		}
	}
}

void UGameplayModeSubsystem::RempoveGameplayMode(FGameplayTagContainer Tags)
{
	for (const FGameplayTag& Tag : Tags)
	{
		if (GameplayModeTag.RemoveTag(Tag))
		{
			if (bCanBroadcast)
			{
				const TPair<FOnGameplayModeTagChanged, int>* FoundHandle = Handles.Find(Tag);
				if (FoundHandle)
				{
					FoundHandle->Key.Broadcast(false);
				}
				OnGameplayModeTagsChanged.Broadcast(Tag, false);
			}
		}
	}
}






TMap<int, TWeakObjectPtr<UObject>>& UGameplayModeSubsystem::GetPriorityItems()
{
	return GameplayModes;
}

void UGameplayModeSubsystem::OnPriorityItemChanged(UObject* PreviousItem, UObject* NewItem)
{

}

void UGameplayModeSubsystem::OnPriorityItemRemoved(UObject* Item, bool bReplaced)
{
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
	
	const UGameplayModeWorldConfig* ModeConfig = AWorldFragmentSettings::GetConfigByClass<UGameplayModeWorldConfig>(GetWorld());
	checkf(IsValid(ModeConfig) && ModeConfig->bEnabled, TEXT("GameplayMode world config is invalid or disabled"));

	SetGameplayModeByTag(ModeConfig->DefaultMode);
}

void UGameplayModeSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	LOG_WARNING(LogTemp, TEXT("GameplayModeSubsystem OnWorldBeginPlay"));

	bCanBroadcast = true;
}

void UGameplayModeSubsystem::Deinitialize()
{
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





#if WITH_EDITOR
EDataValidationResult UGameplayModeWorldConfig::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	if (bEnabled)
	{
		if (DefaultMode.IsEmpty())
		{
			Context.AddError(FText::FromString("Default gameplay mode is empty"));
			return EDataValidationResult::Invalid;
		}
	}

	return Result;
}
#endif


const UGameplayModeWorldConfig* UGameplayModeWorldConfig::Get(UWorld* World)
{
	AWorldFragmentSettings* WorldSettings = Cast<AWorldFragmentSettings>(World->GetWorldSettings());
	if (!IsValid(WorldSettings))
	{
		return nullptr;
	}
	return WorldSettings->FindConfigByClass<UGameplayModeWorldConfig>();
}

