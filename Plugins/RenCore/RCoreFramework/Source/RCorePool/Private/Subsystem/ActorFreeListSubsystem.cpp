// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Subsystem/ActorFreeListSubsystem.h"

// Project Headers
#include "Interface/IActorLinkedNode.h"



AActor* UActorFreeListSubsystem::AcquireFromList(TSubclassOf<AActor> ActorClass, const FTransform& Transform, AActor* Owner)
{
	UWorld* World = GetWorld();

	TObjectPtr<AActor>* StartingNode = ActorList.Find(ActorClass);
	if (StartingNode)
	{
		AActor* Actor = StartingNode->Get();
		IActorLinkedNode* Task = Cast<IActorLinkedNode>(Actor);
		if (Task)
		{
			ActorList.Add(ActorClass, Task->GetNextNode());
			Task->SetNextNode(nullptr);
			Actor->SetOwner(Owner);
			return Actor;
		}
	}

	return World->SpawnActorDeferred<AActor>(ActorClass, Transform, Owner, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
}

void UActorFreeListSubsystem::ReturnToList(AActor* Actor)
{
	IActorLinkedNode* Task = Cast<IActorLinkedNode>(Actor);
	if (!Task)
	{
		return;
	}

	Actor->SetOwner(nullptr);

	UClass* ActorClass = Actor->GetClass();
	if (ActorList.Contains(ActorClass))
	{
		Task->SetNextNode(ActorList.FindChecked(ActorClass));
	}
	ActorList.Add(ActorClass, Actor);
}

void UActorFreeListSubsystem::ClearList()
{
	for (const TPair<TSubclassOf<AActor>, TObjectPtr<AActor>>& Kv : ActorList)
	{
		Kv.Value->Destroy();
	}
	ActorList.Empty();
}

bool UActorFreeListSubsystem::DoesSupportWorldType(EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

void UActorFreeListSubsystem::Deinitialize()
{
	ClearList();
	Super::Deinitialize();
}

UActorFreeListSubsystem* UActorFreeListSubsystem::Get(UWorld* World)
{
	if (IsValid(World))
	{
		return World->GetSubsystem<UActorFreeListSubsystem>();
	}
	return nullptr;
}
