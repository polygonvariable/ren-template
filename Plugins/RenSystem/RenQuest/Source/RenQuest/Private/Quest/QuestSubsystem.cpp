// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Quest/QuestSubsystem.h"

// Engine Headers
#include "EngineUtils.h"
// #include "Kismet/GameplayStatics.h"
// #include "GameFramework/GameModeBase.h"

// Project Headers
#include "RCoreStorage/Public/StorageProviderInterface.h"

#include "RCoreLibrary/Public/SubsystemUtils.h"
#include "RCoreLibrary/Public/LogMacro.h"

#include "RCoreQuest/Public/QuestProviderInterface.h"
#include "RCoreQuest/Public/QuestRecord.h"

#include "RenEventflow/Public/EventflowData.h"
#include "RenEventflow/Public/EventflowEngine.h"
#include "RenEventflow/Public/EventflowNode.h"
#include "RenEventflow/Public/EventflowNodeData.h"
#include "RenEventflow/Public/EventflowNodeTask.h"

#include "RenQuest/Public/Quest/QuestAsset.h"
#include "RenQuest/Public/Quest/QuestEngine.h"
#include "RenQuest/Public/StoryboardAsset.h"





void UQuestSubsystem::StartQuest(UQuestAsset* QuestAsset)
{
	if (!IsValid(QuestAsset))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("QuestAsset is invalid"));
		return;
	}

	TMap<FGuid, FQuestRecord>* ActiveQuests = GetMutableActiveQuests();
	if (!ActiveQuests)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("ActiveQuests is invalid"));
		return;
	}

	if (ActiveQuests->Contains(QuestAsset->QuestId))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Quest is already active"));
		return;
	}
	
	UQuestEngine* QuestEngine = NewObject<UQuestEngine>(this);
	if (!IsValid(QuestEngine))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Failed to create QuestEngine"));
		return;
	}

	FQuestRecord QuestRecord;
	QuestRecord.ActiveObjectives.Add(QuestAsset->GraphData->NodeEntry);

	ActiveQuests->Add(QuestAsset->QuestId, QuestRecord);

	QuestEngines.Add(QuestAsset, QuestEngine);
	QuestEngine->LoadAsset(QuestAsset);
}

void UQuestSubsystem::StartQuest(FGuid QuestId)
{
	UQuestAsset* QuestAsset = GetQuestAsset(QuestId);
	if (IsValid(QuestAsset))
	{
		StartQuest(QuestAsset);
	}
}

void UQuestSubsystem::ResumeQuest(UQuestAsset* QuestAsset, FGuid EntryId)
{
	if (!IsValid(QuestAsset))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("QuestAsset is invalid"));
		return;
	}

	TMap<FGuid, FQuestRecord>* ActiveQuests = GetMutableActiveQuests();
	if (!ActiveQuests)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("ActiveQuests is invalid"));
		return;
	}

	FQuestRecord* QuestRecord = ActiveQuests->Find(QuestAsset->QuestId);
	if (!QuestRecord)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Quest is not active to resume"));
		return;
	}

	UQuestEngine* QuestEngine = NewObject<UQuestEngine>(this);
	if (!IsValid(QuestEngine))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Failed to create QuestEngine"));
		return;
	}

	QuestEngines.Add(QuestAsset, QuestEngine);

	QuestEngine->SetEntryId(EntryId);
	QuestEngine->LoadAsset(QuestAsset);
}


bool UQuestSubsystem::SetObjectiveToCompleted(UQuestAsset* QuestAsset, FGuid ObjectiveId)
{
	FQuestRecord* QuestRecord = GetActiveQuest(QuestAsset);
	if (!QuestRecord)
	{
		return false;
	}

	QuestRecord->ActiveObjectives.Remove(ObjectiveId);
	QuestRecord->CompletedObjectives.AddUnique(ObjectiveId);

	return true;
}

bool UQuestSubsystem::SetObjectiveToActive(UQuestAsset* QuestAsset, FGuid ObjectiveId)
{
	FQuestRecord* QuestRecord = GetActiveQuest(QuestAsset);
	if (!QuestRecord)
	{
		return false;
	}

	QuestRecord->CompletedObjectives.Remove(ObjectiveId);
	QuestRecord->ActiveObjectives.AddUnique(ObjectiveId);

	return true;
}

void UQuestSubsystem::EndQuest(UQuestAsset* QuestAsset)
{
	if (!IsValid(QuestAsset))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("QuestAsset is invalid"));
		return;
	}

	TMap<FGuid, FQuestRecord>* ActiveQuests = GetMutableActiveQuests();
	if (!ActiveQuests)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("ActiveQuests is invalid"));
		return;
	}

	if (!ActiveQuests->Contains(QuestAsset->QuestId))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Quest is not active to end"));
		return;
	}

	UQuestEngine* QuestEngine = QuestEngines.FindRef(QuestAsset);
	if (!QuestEngine)
	{
		PRINT_WARNING(LogTemp, 1.0f, TEXT("Quest is not active"));
		return;
	}

	if (IsValid(QuestEngine))
	{
		QuestEngine->UnloadAsset();
		QuestEngine->MarkAsGarbage();
	}

	QuestEngines.Remove(QuestAsset);
	ActiveQuests->Remove(QuestAsset->QuestId);
}


bool UQuestSubsystem::IsQuestActive(UQuestAsset* QuestAsset) const
{
	return (GetActiveQuest(QuestAsset) != nullptr && GetCompletedQuest(QuestAsset) == nullptr);
}

bool UQuestSubsystem::IsQuestCompleted(UQuestAsset* QuestAsset) const
{
	return (GetCompletedQuest(QuestAsset) != nullptr && GetActiveQuest(QuestAsset) == nullptr);
}


void UQuestSubsystem::AddAvailableQuest(UQuestAsset* QuestAsset)
{
	if (!QuestAsset) return;
	if (AvailableQuests.Contains(QuestAsset->QuestId)) return;

	AvailableQuests.Add(QuestAsset->QuestId, QuestAsset);
}

FQuestRecord* UQuestSubsystem::GetActiveQuest(UQuestAsset* QuestAsset) const
{
	if (!IsValid(QuestAsset))
	{
		return nullptr;
	}

	TMap<FGuid, FQuestRecord>* ActiveQuests = GetMutableActiveQuests();
	if (!ActiveQuests)
	{
		return nullptr;
	}

	return ActiveQuests->Find(QuestAsset->QuestId);
}

FDateTime* UQuestSubsystem::GetCompletedQuest(UQuestAsset* QuestAsset) const
{
	if (!IsValid(QuestAsset))
	{
		return nullptr;
	}

	TMap<FGuid, FDateTime>* CompletedQuests = GetMutableCompletedQuests();
	if (!CompletedQuests)
	{
		return nullptr;
	}

	return CompletedQuests->Find(QuestAsset->QuestId);
}

bool UQuestSubsystem::IsObjectiveCompleted(UQuestAsset* QuestAsset, FGuid ObjectiveId) const
{
	FQuestRecord* QuestRecord = GetActiveQuest(QuestAsset);
	if (!QuestRecord)
	{
		return false;
	}

	return QuestRecord->CompletedObjectives.Contains(ObjectiveId);
}

bool UQuestSubsystem::IsObjectiveActive(UQuestAsset* QuestAsset, FGuid ObjectiveId) const
{
	FQuestRecord* QuestRecord = GetActiveQuest(QuestAsset);
	if (!QuestRecord)
	{
		return false;
	}

	return QuestRecord->ActiveObjectives.Contains(ObjectiveId);
}


const TMap<FGuid, FQuestRecord>* UQuestSubsystem::GetActiveQuests() const
{
	IQuestProviderInterface* QuestInterface = QuestProvider.Get();
	if (!QuestInterface)
	{
		return nullptr;
	}

	return &QuestInterface->GetActiveQuests();
}

TMap<FGuid, FQuestRecord>* UQuestSubsystem::GetMutableActiveQuests() const
{
	IQuestProviderInterface* QuestInterface = QuestProvider.Get();
	if (!QuestInterface)
	{
		return nullptr;
	}

	return &QuestInterface->GetMutableActiveQuests();
}

const TMap<FGuid, FDateTime>* UQuestSubsystem::GetCompletedQuests() const
{
	IQuestProviderInterface* QuestInterface = QuestProvider.Get();
	if (!QuestInterface)
	{
		return nullptr;
	}

	return &QuestInterface->GetCompletedQuests();
}

TMap<FGuid, FDateTime>* UQuestSubsystem::GetMutableCompletedQuests() const
{
	IQuestProviderInterface* QuestInterface = QuestProvider.Get();
	if (!QuestInterface)
	{
		return nullptr;
	}

	return &QuestInterface->GetMutableCompletedQuests();
}


UQuestAsset* UQuestSubsystem::GetQuestAsset(FGuid QuestId) const
{
	const TObjectPtr<UQuestAsset>* FoundQuest = AvailableQuests.Find(QuestId);
	if (!FoundQuest)
	{
		return nullptr;
	}

	return FoundQuest->Get();
}

void UQuestSubsystem::HandleStorageLoaded()
{
	LOG_INFO(LogTemp, TEXT("QuestSubsystem storage load started"));

	IStorageProviderInterface* StorageInterface = SubsystemUtils::GetSubsystemInterface<UGameInstance, UGameInstanceSubsystem, IStorageProviderInterface>(GetWorld()->GetGameInstance());
	if (!StorageInterface)
	{
		LOG_ERROR(LogTemp, TEXT("StorageInterface is invalid"));
		return;
	}

	UObject* Storage = StorageInterface->GetLocalStorage();
	IQuestProviderInterface* QuestInterface = Cast<IQuestProviderInterface>(Storage);
	if (!QuestInterface)
	{
		LOG_ERROR(LogTemp, TEXT("QuestProvider is invalid or does not implement IQuestProviderInterface"));
		return;
	}

	QuestProvider = TWeakInterfacePtr<IQuestProviderInterface>(QuestInterface);
	LOG_INFO(LogTemp, TEXT("QuestSubsystem storage loaded"));

	const TMap<FGuid, FQuestRecord>& ActiveQuests = QuestInterface->GetActiveQuests();
	for (const auto& Kv : ActiveQuests)
	{
		UQuestAsset* QuestAsset = GetQuestAsset(Kv.Key);
		const FQuestRecord& QuestRecord = Kv.Value;

		ResumeQuest(QuestAsset, QuestRecord.ActiveObjectives[0]);
	}
}

bool UQuestSubsystem::DoesSupportWorldType(EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

void UQuestSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogTemp, TEXT("QuestSubsystem initialized"));

	//HandleStorageLoaded();
}

void UQuestSubsystem::OnWorldComponentsUpdated(UWorld& InWorld)
{
	Super::OnWorldComponentsUpdated(InWorld);
}

void UQuestSubsystem::Deinitialize()
{
	QuestProvider.Reset();

	LOG_WARNING(LogTemp, TEXT("QuestSubsystem deinitialized"));
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


