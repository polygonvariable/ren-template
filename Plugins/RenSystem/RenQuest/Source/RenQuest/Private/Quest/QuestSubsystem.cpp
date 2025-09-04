// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Quest/QuestSubsystem.h"

// Engine Headers
#include "EngineUtils.h"
// #include "Kismet/GameplayStatics.h"
// #include "GameFramework/GameModeBase.h"

// Project Headers
#include "RenCoreLibrary/Public/LogMacro.h"

#include "RenEventflow/Public/EventflowData.h"
#include "RenEventflow/Public/EventflowEngine.h"
#include "RenEventflow/Public/EventflowNode.h"
#include "RenEventflow/Public/EventflowNodeData.h"
#include "RenEventflow/Public/EventflowNodeTask.h"

#include "RenQuest/Public/Quest/QuestAsset.h"
#include "RenQuest/Public/StoryboardAsset.h"





void UQuestSubsystem::StartQuest(UQuestAsset* QuestAsset)
{
	if (ActiveQuests.Contains(QuestAsset))
	{
		PRINT_WARNING(LogTemp, 1.0f, TEXT("Quest is already active"));
		return;
	}
	
	UEventflowEngine* EventflowEngine = NewObject<UEventflowEngine>(this);

	ActiveQuests.Add(QuestAsset, EventflowEngine);

	EventflowEngine->OnNodeReached.BindWeakLambda(this, [this, Engine = EventflowEngine](UEventflowNode* Node)
		{
			/*
			 * The binded node event should have "Node_" prefix and takes a UEventflowNode* parameter
			 * The engine expects a callback event which trigger OnNodeExited
			 */
			Engine->ExecuteNode(Node);
		}
	);
	EventflowEngine->OnNodeExited.BindWeakLambda(this, [this, Engine = EventflowEngine](UEventflowNode* Node, bool bSuccess)
		{
			if (bSuccess)
			{
				this->FocusedQuest = Cast<UQuestAsset>(Engine->GetOwningAsset());
				Engine->ReachImmediateNextNode();
			}
		}
	);
	EventflowEngine->OnGraphEnded.BindWeakLambda(this, [this, Engine = EventflowEngine]()
		{
			this->EndQuest(Cast<UQuestAsset>(Engine->GetOwningAsset()));
		}
	);
	EventflowEngine->OnEngineInitialized.BindWeakLambda(this, [this, Engine = EventflowEngine]()
		{
			Engine->ReachEntryNode();
		}
	);

	EventflowEngine->LoadAsset(QuestAsset);
}

void UQuestSubsystem::EndQuest(UQuestAsset* QuestAsset)
{
	UEventflowEngine* EventflowEngine = ActiveQuests.FindRef(QuestAsset);
	if (!EventflowEngine)
	{
		PRINT_WARNING(LogTemp, 1.0f, TEXT("Quest is not active"));
		return;
	}

	if (IsValid(EventflowEngine))
	{
		EventflowEngine->UnloadAsset();

		EventflowEngine->OnNodeReached.Unbind();
		EventflowEngine->OnNodeExited.Unbind();
		EventflowEngine->OnGraphEnded.Unbind();
		EventflowEngine->OnEngineInitialized.Unbind();

		EventflowEngine->MarkAsGarbage();
	}

	ActiveQuests.Remove(QuestAsset);
}

bool UQuestSubsystem::IsQuestActive(UQuestAsset* QuestAsset) const
{
	//return ActiveQuests.Contains(QuestAsset);
	return false;
}

bool UQuestSubsystem::IsQuestCompleted(UQuestAsset* QuestAsset) const
{
	//return CompletedQuests.Contains(QuestAsset);
	return false;
}



bool UQuestSubsystem::DoesSupportWorldType(EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

void UQuestSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UQuestSubsystem::OnWorldComponentsUpdated(UWorld& InWorld)
{
	Super::OnWorldComponentsUpdated(InWorld);
}

void UQuestSubsystem::Deinitialize()
{
	Super::Deinitialize();
}


/*
void UObjectPoolX::InitializePool(TSubclassOf<UObject> InClass, int32 InitialSize)
{
	PooledClass = InClass;
}

UObject* UObjectPoolX::CreateNewObject()
{
	if (!PooledClass) return nullptr;

	return NewObject<UObject>(this, PooledClass);
}

UObject* UObjectPoolX::Acquire()
{
	if (AvailableObjects.Num() == 0)
	{
		return CreateNewObject();
	}

	return AvailableObjects.Pop();
}

void UObjectPoolX::Release(UObject* Object)
{
	if (Object)
	{
		AvailableObjects.Add(Object);
	}
}
AQuestNetworkSync::AQuestNetworkSync()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bNetLoadOnClient = true;
}
AActor* UNetworkSyncComponent::GetNetworkActor(TSubclassOf<AActor> InClass)
{
	UWorld* World = GetWorld();
	if (!InClass || !World)
	{
		return nullptr;
	}
	
	if (TWeakObjectPtr<AActor> const* Found = CachedNetworkActors.Find(InClass))
	{
		if (Found->IsValid())
		{
			return Found->Get();
		}
	}

	for (TActorIterator<AActor> ActorItr(World); ActorItr; ++ActorItr)
	{
		if (ActorItr->IsA(InClass))
		{
			CachedNetworkActors.Add(InClass, *ActorItr);
			return *ActorItr;
		}
	}

	return nullptr;
}
UWorld* UNetworkBlueprintLibrary::GetWorldObject(UObject* Object)
{
	if (!Object) return nullptr;
	return Object->GetWorld();
}
UNetworkSyncComponent* UNetworkBlueprintLibrary::GetNetworkSync(UWorld* WorldContextObject, TSubclassOf<UNetworkSyncComponent> InClass)
{
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(WorldContextObject);
	if (!GameMode) return nullptr;

	UActorComponent* ActorComponent = GameMode->FindComponentByClass(InClass);
	return Cast<UNetworkSyncComponent>(ActorComponent);
}
UNetworkRouterComponent* UNetworkBlueprintLibrary::GetNetworkRouter(UWorld* WorldContextObject, TSubclassOf<UNetworkRouterComponent> InClass)
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	if (!PC) return nullptr;

	UActorComponent* ActorComponent = PC->FindComponentByClass(InClass);
	return Cast<UNetworkRouterComponent>(ActorComponent);
}

*/


