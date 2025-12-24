// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbilityTypes.h"

// Generated Headers
#include "GameplayEventNotifyState.generated.h"



/**
 * 
 */
UCLASS()
class RENABILITY_API UGameplayEventNotifyState : public UAnimNotifyState
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag EventStartTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag EventTickTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag EventEndTag;

	FGameplayEventData EventPayload;

	void SendEvent(USkeletalMeshComponent* MeshComp, const FGameplayTag& EventTag);

public:

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

};

