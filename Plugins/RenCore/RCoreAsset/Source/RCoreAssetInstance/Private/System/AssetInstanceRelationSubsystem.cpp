// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/AssetInstanceRelationSubsystem.h"

// Project Headers
#include "Core/Interface/AssetInstanceRelation.h"
#include "Log/LogMacro.h"


void UAssetInstanceRelationSubsystem::RegisterRelation(const FPrimaryAssetType& AssetType, IAssetInstanceRelation* Relation)
{
	if (AssetType.IsValid() && Relation)
	{
		RelationMap.Add(AssetType, Relation);

		LOG_WARNING(LogTemp, TEXT("Register Relation: %s"), *AssetType.ToString());
	}
}

void UAssetInstanceRelationSubsystem::UnregisterRelation(const FPrimaryAssetType& AssetType, IAssetInstanceRelation* Relation)
{
	RelationMap.Remove(AssetType, Relation);

	LOG_WARNING(LogTemp, TEXT("Unregister Relation: %s"), *AssetType.ToString());
}

bool UAssetInstanceRelationSubsystem::HasAnyLinkedRelation(const FPrimaryAssetId& AssetId, const FGuid& AssetInstanceId) const
{
	TArray<const TWeakInterfacePtr<IAssetInstanceRelation>*> FoundRelations;
	RelationMap.MultiFindPointer(AssetId.PrimaryAssetType, FoundRelations);

	for (const TWeakInterfacePtr<IAssetInstanceRelation>* Relation : FoundRelations)
	{
		if (!Relation)
		{
			continue;
		}

		IAssetInstanceRelation* InstanceRelation = Relation->Get();
		if (InstanceRelation && InstanceRelation->HasLinkedInstance(AssetId, AssetInstanceId))
		{
			return true;
		}
	}

	return false;
}

bool UAssetInstanceRelationSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UAssetInstanceRelationSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UAssetInstanceRelationSubsystem::Deinitialize()
{
	Super::Deinitialize();
}


UAssetInstanceRelationSubsystem* UAssetInstanceRelationSubsystem::Get(UWorld* World)
{
	if (!IsValid(World))
	{
		return nullptr;
	}
	return Get(World->GetGameInstance());
}

UAssetInstanceRelationSubsystem* UAssetInstanceRelationSubsystem::Get(UGameInstance* GameInstance)
{
	if (!IsValid(GameInstance))
	{
		return nullptr;
	}
	return GameInstance->GetSubsystem<UAssetInstanceRelationSubsystem>();
}

