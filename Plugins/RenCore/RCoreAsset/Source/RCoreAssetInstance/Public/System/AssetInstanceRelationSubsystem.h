// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/GameInstanceSubsystem.h"

// Generated Headers
#include "AssetInstanceRelationSubsystem.generated.h"

// Module Macros
#define REN_API RCOREASSETINSTANCE_API

// Forward Declarations
class IAssetInstanceRelation;


/**
 * Rename to AssetInstanceReference
 */
UCLASS(MinimalAPI)
class UAssetInstanceRelationSubsystem : public UGameInstanceSubsystem
{

	GENERATED_BODY()

public:

	REN_API void RegisterRelation(const FPrimaryAssetType& AssetType, IAssetInstanceRelation* Relation);
	REN_API void UnregisterRelation(const FPrimaryAssetType& AssetType, IAssetInstanceRelation* Relation);

	REN_API bool HasAnyLinkedRelation(const FPrimaryAssetId& AssetId, const FGuid& AssetInstanceId) const;

protected:

    TMultiMap<FPrimaryAssetType, TWeakInterfacePtr<IAssetInstanceRelation>> RelationMap;

	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem

public:

	static REN_API UAssetInstanceRelationSubsystem* Get(UWorld* World);
	static REN_API UAssetInstanceRelationSubsystem* Get(UGameInstance* GameInstance);

};


// Module Macros
#undef REN_API

