// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "StructUtils/InstancedStruct.h"

// Project Headers
#include "GameplayContextAction.h"
#include "GameplayContextInterface.h"

// Generated Headers
#include "GameplayContextComponent.generated.h"


/**
 *
 */
UCLASS(MinimalAPI, meta = (BlueprintSpawnableComponent))
class UGameplayContextComponent : public UActorComponent, public IGameplayContextInterface
{

	GENERATED_BODY()

public:

	UGameplayContextComponent();

	UPROPERTY(EditAnywhere, meta = (ExcludeBaseStruct))
	TArray<TInstancedStruct<FGameplayContextAction>> Contexts;


	// ~ IGameplayContextInterface
	virtual void PushContext(TInstancedStruct<FGameplayContextAction>&& Context) override;
	virtual void Execute(const FGameplayTagContainer& ContextTags, UObject* Caller) override;
	// ~ End of IGameplayContextInterface

};

