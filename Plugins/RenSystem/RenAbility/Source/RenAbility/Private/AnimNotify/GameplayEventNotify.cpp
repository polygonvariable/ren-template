// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "AnimNotify/GameplayEventNotify.h"

// Engine Headers
#include "AbilitySystemBlueprintLibrary.h"



void UGameplayEventNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AActor* Owner = MeshComp->GetOwner();
	if (Owner)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Owner, EventTag, EventPayload);
	}
}

