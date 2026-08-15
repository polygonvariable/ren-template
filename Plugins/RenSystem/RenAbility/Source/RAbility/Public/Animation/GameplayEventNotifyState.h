// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Abilities/GameplayAbilityTypes.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GameplayTagContainer.h"

// Generated Headers
#include "GameplayEventNotifyState.generated.h"


/**
 * 
 */
UCLASS()
class UGameplayEventNotifyState : public UAnimNotifyState
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Categories = "Event"))
	FGameplayTag EventStartTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Categories = "Event"))
	FGameplayTag EventTickTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Categories = "Event"))
	FGameplayTag EventEndTag;

	FGameplayEventData EventPayload;


	void SendEvent(USkeletalMeshComponent* MeshComp, const FGameplayTag& EventTag);

	// ~ UAnimNotifyState
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	// ~ End of UAnimNotifyState

};

