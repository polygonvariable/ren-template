// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Subsystem/BroadcastSubsystem.h"


void UBroadcastSubsystem::RegisterBroadcast(FGameplayTag Tag, FBroadcastDelegate::FDelegate&& Callback)
{
	TPair<FBroadcastDelegate, int>& Pair = BroadcastMap.FindOrAdd(Tag);
	Pair.Key.Add(Callback);
	Pair.Value++;

	UE_LOG(LogTemp, Log, TEXT("Subscribe: %s"), *Tag.ToString());
}

void UBroadcastSubsystem::UnregisterBroadcast(const FGameplayTag& Tag, UObject* Target)
{
	TPair<FBroadcastDelegate, int>* Pair = BroadcastMap.Find(Tag);
	if (Pair)
	{
		Pair->Key.RemoveAll(Target);
		Pair->Value--;
		
		if (Pair->Value == 0)
		{
			Pair->Key.Clear();
			BroadcastMap.Remove(Tag);

			UE_LOG(LogTemp, Log, TEXT("Tag broadcast removed"));
		}
	}

	UE_LOG(LogTemp, Log, TEXT("UnregisterBroadcast: %s"), *Tag.ToString());
}

void UBroadcastSubsystem::BroadcastMessage(const FGameplayTag& Tag, const FInstancedStruct& Payload)
{
	TPair<FBroadcastDelegate, int>* Pair = BroadcastMap.Find(Tag);
	if (Pair)
	{
		Pair->Key.Broadcast(Payload);
	}
	UE_LOG(LogTemp, Log, TEXT("BroadcastMessage: %s"), *Tag.ToString());
}

UBroadcastSubsystem* UBroadcastSubsystem::Get(UWorld* World)
{
	if (!IsValid(World))
	{
		return nullptr;
	}
	return Get(World->GetGameInstance());
}

UBroadcastSubsystem* UBroadcastSubsystem::Get(UGameInstance* GameInstance)
{
	if (!IsValid(GameInstance))
	{
		return nullptr;
	}
	return GameInstance->GetSubsystem<UBroadcastSubsystem>();
}

