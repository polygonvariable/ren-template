// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbilityTypes.h"

// Generated Headers
#include "GameplayEventNotify.generated.h"



/**
 * 
 * 
 */
UCLASS()
class RENABILITY_API UGameplayEventNotify : public UAnimNotify
{

	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag EventTag;

	FGameplayEventData EventPayload;

public:

	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

};

