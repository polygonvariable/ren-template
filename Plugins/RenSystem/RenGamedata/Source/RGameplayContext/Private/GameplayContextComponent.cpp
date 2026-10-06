// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "GameplayContextComponent.h"

// Engine Headers
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif


UGameplayContextComponent::UGameplayContextComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	bAutoActivate = false;
}

void UGameplayContextComponent::PushContext(TInstancedStruct<FGameplayContextAction>&& Context)
{
	Contexts.Add(MoveTemp(Context));
}

void UGameplayContextComponent::Execute(const FGameplayTagContainer& ContextTags, UObject* Caller)
{
	UWorld* World = GetWorld();

	for (TInstancedStruct<FGameplayContextAction>& ContextData : Contexts)
	{
		FGameplayContextAction* Data = ContextData.GetMutablePtr();
		if (Data && Data->ContextTags.HasAnyExact(ContextTags))
		{
			Data->Execute(World, Caller);
		}
	}
}

