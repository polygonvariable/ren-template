// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "AnimNotify/GameplayEventNotifyState.h"

// Engine Headers
#include "AbilitySystemBlueprintLibrary.h"



void UGameplayEventNotifyState::SendEvent(USkeletalMeshComponent* MeshComp, const FGameplayTag& EventTag)
{
	if (AActor* Owner = MeshComp->GetOwner())
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Owner, EventTag, EventPayload);
	}
}

void UGameplayEventNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	SendEvent(MeshComp, EventStartTag);
}

void UGameplayEventNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	SendEvent(MeshComp, EventTickTag);
}

void UGameplayEventNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	SendEvent(MeshComp, EventEndTag);
}

