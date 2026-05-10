// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Subsystem/ActorFreelistSubsystem.h"

// Project Headers
#include "Interface/IActorLinkedNode.h"


AActor* UActorFreelistSubsystem::AcquireFromList(TSubclassOf<AActor> ActorClass, const FTransform& Transform, AActor* Owner)
{
	UWorld* World = GetWorld();

	TObjectPtr<AActor>* StartingNode = ActorList.Find(ActorClass);
	if (StartingNode)
	{
		AActor* Actor = StartingNode->Get();
		IActorLinkedNode* Node = Cast<IActorLinkedNode>(Actor);
		if (Node)
		{
			ActorList.Add(ActorClass, Node->GetNextNode());
			Node->SetNextNode(nullptr);
			Actor->SetOwner(Owner);
			return Actor;
		}
	}

	return World->SpawnActorDeferred<AActor>(ActorClass, Transform, Owner, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
}

void UActorFreelistSubsystem::ReturnToList(AActor* Actor)
{
	IActorLinkedNode* Node = Cast<IActorLinkedNode>(Actor);
	if (!Node)
	{
		return;
	}

	Actor->SetOwner(nullptr);

	UClass* ActorClass = Actor->GetClass();
	if (ActorList.Contains(ActorClass))
	{
		Node->SetNextNode(ActorList.FindChecked(ActorClass));
	}
	ActorList.Add(ActorClass, Actor);
}

void UActorFreelistSubsystem::ClearList()
{
	for (const TPair<TSubclassOf<AActor>, TObjectPtr<AActor>>& Kv : ActorList)
	{
		if (IsValid(Kv.Value))
		{
			Kv.Value->Destroy();
		}
	}
	ActorList.Empty();
}

bool UActorFreelistSubsystem::DoesSupportWorldType(EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

void UActorFreelistSubsystem::Deinitialize()
{
	ClearList();
	Super::Deinitialize();
}

UActorFreelistSubsystem* UActorFreelistSubsystem::Get(UWorld* World)
{
	if (IsValid(World))
	{
		return World->GetSubsystem<UActorFreelistSubsystem>();
	}
	return nullptr;
}

