// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "QuestReactiveComponent.h"

// Project Headers
#include "GameplayContextInterface.h"
#include "System/QuestSubsystem.h"


UQuestReactiveComponent::UQuestReactiveComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	bAutoActivate = false;
}

void UQuestReactiveComponent::BeginPlay()
{
	Super::BeginPlay();

	Subsystem = UQuestSubsystem::Get(GetWorld());
	if (IsValid(Subsystem))
	{
		Subsystem->GetOnEngineAdded().AddUObject(this, &UQuestReactiveComponent::HandleOnQuestChanged);
		Subsystem->GetOnEngineRemoved().AddUObject(this, &UQuestReactiveComponent::HandleOnQuestChanged);
	}
}

void UQuestReactiveComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (IsValid(Subsystem))
	{
		Subsystem->GetOnEngineAdded().RemoveAll(this);
		Subsystem->GetOnEngineRemoved().RemoveAll(this);
	}

	Super::EndPlay(EndPlayReason);
}

void UQuestReactiveComponent::HandleOnQuestChanged(FPrimaryAssetId AssetId, UEventflowEngine* Engine)
{
	if (QuestId == AssetId)
	{
		UActorComponent* ContextComponent = GetOwner()->FindComponentByInterface(UGameplayContextInterface::StaticClass());
		if (!IsValid(ContextComponent))
		{
			ensureMsgf(false, TEXT("Owner actor doesnt component with gameplay context"));
			return;
		}

		IGameplayContextInterface* ContextInterface = Cast<IGameplayContextInterface>(ContextComponent);
		if (ContextInterface)
		{
			ContextInterface->Execute(ContextTags, this);
		}
	}
}

