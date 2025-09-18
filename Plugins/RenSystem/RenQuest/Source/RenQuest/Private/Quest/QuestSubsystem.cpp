// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Quest/QuestSubsystem.h"

// Engine Headers
#include "Engine/AssetManager.h"

// Project Headers
#include "RCoreStorage/Public/StorageProviderInterface.h"

#include "RCoreLibrary/Public/AssetManagerUtils.h"
#include "RCoreLibrary/Public/LogMacro.h"
#include "RCoreLibrary/Public/SubsystemUtils.h"

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





void UQuestSubsystem::StartQuest(FPrimaryAssetId QuestId)
{
	if (!QuestId.IsValid())
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("QuestId is invalid"));
		return;
	}
	
	FName QuestName = QuestId.PrimaryAssetName;
	if (AssetManagerUtils::GetAssetTagValue<bool>(AssetManager, QuestId, TEXT("bIsResumable")))
	{
		if (!AddQuest(QuestName))
		{
			PRINT_ERROR(LogTemp, 1.0f, TEXT("Failed to add quest"));
			return;
		}
	}

	UQuestEngine* QuestEngine = NewObject<UQuestEngine>(this);
	if (!IsValid(QuestEngine))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Failed to create QuestEngine"));
		return;
	}

	QuestEngines.Add(QuestName, QuestEngine);

	QuestEngine->LoadAsset(QuestId);
}

void UQuestSubsystem::ResumeQuest(const FPrimaryAssetId& QuestId, const TArray<FGuid>& EntryIds)
{
	if (!QuestId.IsValid())
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("QuestId is invalid"));
		return;
	}

	const TMap<FName, FQuestRecord>* ActiveQuests = GetActiveQuests();
	if (!ActiveQuests)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("ActiveQuests is invalid"));
		return;
	}

	FName QuestName = QuestId.PrimaryAssetName;
	if (!ActiveQuests->Contains(QuestName))
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

	QuestEngines.Add(QuestName, QuestEngine);

	QuestEngine->SetEntryIds(EntryIds);
	QuestEngine->LoadAsset(QuestId);
}




bool UQuestSubsystem::SetObjectiveToCompleted(const FPrimaryAssetId& QuestId, FGuid ObjectiveId)
{
	FQuestRecord* QuestRecord = GetActiveQuest(QuestId);
	if (!QuestRecord)
	{
		return false;
	}

	QuestRecord->ActiveObjectives.Remove(ObjectiveId);
	QuestRecord->CompletedObjectives.AddUnique(ObjectiveId);

	return true;
}

bool UQuestSubsystem::SetObjectiveToActive(const FPrimaryAssetId& QuestId, FGuid ObjectiveId)
{
	FQuestRecord* QuestRecord = GetActiveQuest(QuestId);
	if (!QuestRecord)
	{
		return false;
	}

	QuestRecord->CompletedObjectives.Remove(ObjectiveId);
	QuestRecord->ActiveObjectives.AddUnique(ObjectiveId);

	return true;
}







void UQuestSubsystem::EndQuest(FPrimaryAssetId QuestId)
{
	if (!QuestId.IsValid())
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("QuestId is invalid"));
		return;
	}

	FName QuestName = QuestId.PrimaryAssetName;
	if (AssetManagerUtils::GetAssetTagValue<bool>(AssetManager, QuestId, TEXT("bIsResumable")))
	{
		if (!RemoveQuest(QuestName))
		{
			PRINT_ERROR(LogTemp, 1.0f, TEXT("Failed to remove quest"));
			return;
		}
	}

	TObjectPtr<UQuestEngine>* FoundEngine = QuestEngines.Find(QuestName);
	if (FoundEngine)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("QuestEngine is not found"));
		return;
	}

	UQuestEngine* QuestEngine = FoundEngine->Get();
	if (IsValid(QuestEngine))
	{
		QuestEngine->UnloadAsset();
		QuestEngine->MarkAsGarbage();
	}

	QuestEngines.Remove(QuestName);
}


bool UQuestSubsystem::IsQuestActive(const FPrimaryAssetId& QuestId) const
{
	return (GetActiveQuest(QuestId) != nullptr && GetCompletedQuest(QuestId) == nullptr);
}

bool UQuestSubsystem::IsQuestCompleted(const FPrimaryAssetId& QuestId) const
{
	return (GetCompletedQuest(QuestId) != nullptr && GetActiveQuest(QuestId) == nullptr);
}




FQuestRecord* UQuestSubsystem::GetActiveQuest(const FPrimaryAssetId& QuestId) const
{
	TMap<FName, FQuestRecord>* ActiveQuests = GetMutableActiveQuests();
	if (!ActiveQuests)
	{
		return nullptr;
	}

	return ActiveQuests->Find(QuestId.PrimaryAssetName);
}

FDateTime* UQuestSubsystem::GetCompletedQuest(const FPrimaryAssetId& QuestId) const
{
	TMap<FName, FDateTime>* CompletedQuests = GetMutableCompletedQuests();
	if (!CompletedQuests)
	{
		return nullptr;
	}

	return CompletedQuests->Find(QuestId.PrimaryAssetName);
}

bool UQuestSubsystem::IsObjectiveCompleted(const FPrimaryAssetId& QuestId, FGuid ObjectiveId) const
{
	FQuestRecord* QuestRecord = GetActiveQuest(QuestId);
	if (!QuestRecord)
	{
		return false;
	}

	return QuestRecord->CompletedObjectives.Contains(ObjectiveId);
}

bool UQuestSubsystem::IsObjectiveActive(const FPrimaryAssetId& QuestId, FGuid ObjectiveId) const
{
	FQuestRecord* QuestRecord = GetActiveQuest(QuestId);
	if (!QuestRecord)
	{
		return false;
	}

	return QuestRecord->ActiveObjectives.Contains(ObjectiveId);
}


const TMap<FName, FQuestRecord>* UQuestSubsystem::GetActiveQuests() const
{
	IQuestProviderInterface* QuestProviderInterface = QuestProvider.Get();
	if (!QuestProviderInterface)
	{
		return nullptr;
	}

	return &QuestProviderInterface->GetActiveQuests();
}

TMap<FName, FQuestRecord>* UQuestSubsystem::GetMutableActiveQuests() const
{
	IQuestProviderInterface* QuestProviderInterface = QuestProvider.Get();
	if (!QuestProviderInterface)
	{
		return nullptr;
	}

	return &QuestProviderInterface->GetMutableActiveQuests();
}

const TMap<FName, FDateTime>* UQuestSubsystem::GetCompletedQuests() const
{
	IQuestProviderInterface* QuestProviderInterface = QuestProvider.Get();
	if (!QuestProviderInterface)
	{
		return nullptr;
	}

	return &QuestProviderInterface->GetCompletedQuests();
}


bool UQuestSubsystem::AddQuest(FName QuestName)
{
	TMap<FName, FQuestRecord>* ActiveQuests = GetMutableActiveQuests();
	if (!ActiveQuests)
	{
		LOG_ERROR(LogTemp, TEXT("ActiveQuests is invalid"));
		return false;
	}

	if (ActiveQuests->Contains(QuestName))
	{
		LOG_ERROR(LogTemp, TEXT("Quest is already active"));
		return false;
	}

	ActiveQuests->Add(QuestName, FQuestRecord());
	return true;
}

bool UQuestSubsystem::RemoveQuest(FName QuestName)
{
	TMap<FName, FQuestRecord>* ActiveQuests = GetMutableActiveQuests();
	if (!ActiveQuests)
	{
		LOG_ERROR(LogTemp, TEXT("ActiveQuests is invalid"));
		return false;
	}
	ActiveQuests->Remove(QuestName);

	TMap<FName, FDateTime>* CompletedQuests = GetMutableCompletedQuests();
	if (!CompletedQuests)
	{
		LOG_ERROR(LogTemp, TEXT("CompletedQuests is invalid"));
		return false;
	}
	CompletedQuests->Add(QuestName, FDateTime::Now());

	return true;
}


TMap<FName, FDateTime>* UQuestSubsystem::GetMutableCompletedQuests() const
{
	IQuestProviderInterface* QuestProviderInterface = QuestProvider.Get();
	if (!QuestProviderInterface)
	{
		return nullptr;
	}

	return &QuestProviderInterface->GetMutableCompletedQuests();
}



void UQuestSubsystem::LoadQuestProvider(UWorld& InWorld)
{
	LOG_INFO(LogTemp, TEXT("QuestSubsystem storage load started"));

	IStorageProviderInterface* StorageInterface = SubsystemUtils::GetSubsystemInterface<UGameInstance, UGameInstanceSubsystem, IStorageProviderInterface>(InWorld.GetGameInstance());
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
}

void UQuestSubsystem::ResumeSavedQuests()
{
	IQuestProviderInterface* QuestProviderInterface = QuestProvider.Get();
	if (!QuestProviderInterface)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("QuestProvider is invalid"));
		return;
	}

	const TMap<FName, FQuestRecord>& ActiveQuests = QuestProviderInterface->GetActiveQuests();
	for (const auto& Kv : ActiveQuests)
	{
		const FQuestRecord& QuestRecord = Kv.Value;
		FPrimaryAssetId QuestId = FPrimaryAssetId("Quest", Kv.Key);

		ResumeQuest(QuestId, QuestRecord.ActiveObjectives);
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

	AssetManager = UAssetManager::GetIfInitialized();
}

void UQuestSubsystem::OnWorldComponentsUpdated(UWorld& InWorld)
{
	Super::OnWorldComponentsUpdated(InWorld);
	LOG_WARNING(LogTemp, TEXT("QuestSubsystem world components updated"));

	LoadQuestProvider(InWorld);
	ResumeSavedQuests();
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


