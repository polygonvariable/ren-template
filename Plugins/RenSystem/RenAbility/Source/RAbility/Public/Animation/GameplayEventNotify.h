// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Abilities/GameplayAbilityTypes.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"

// Generated Headers
#include "GameplayEventNotify.generated.h"


/**
 * 
 */
UCLASS()
class UGameplayEventNotify : public UAnimNotify
{

	GENERATED_BODY()
	
public:

	UGameplayEventNotify();


	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Categories = "Event,Equipment.Event"))
	FGameplayTag EventTag;

	// ~ UAnimNotify
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	// ~ End of UAnimNotify

protected:

	FGameplayEventData EventPayload;

};

