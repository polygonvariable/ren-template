// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "MaterialSurfaceProperty.h"
#include "Priority/PriorityListInterface.h"

// Generated Headers
#include "SeasonController.generated.h"

// Module Macros
#define REN_API RSEASON_API

// Forward Declarations
class UMaterialParameterCollectionInstance;
class USeasonCollectionAsset;
class USeasonAsset;
class UEnvironmentSubsystem;


/**
 * 
 */
UCLASS(MinimalAPI)
class USeasonController : public UObject, public IPriorityListInterface
{

	GENERATED_BODY()

public:

	void Initialize(UMaterialParameterCollectionInstance* InMPCInstance);
	void Deinitialize();

	bool AddSeason(USeasonCollectionAsset* SeasonCollection, int Priority);
	bool RemoveSeason(int Priority);

#if UE_BUILD_DEVELOPMENT
	REN_API int GetEditorYearLength() const;
	REN_API int GetEditorCurrentDay() const;
	REN_API const USeasonCollectionAsset* GetEditorCurrentSeasonCollection() const;
	REN_API const TMap<int, TWeakObjectPtr<UObject>>& GetEditorSeasonCollectionList() const;
#endif

protected:

	UPROPERTY()
	TObjectPtr<UMaterialParameterCollectionInstance>  MPCInstance;

	UPROPERTY()
	TObjectPtr<UEnvironmentSubsystem> EnvironmentSubsystem;

	UPROPERTY()
	TObjectPtr<USeasonCollectionAsset> CurrentCollection;

	FTimerHandle TimerHandle;
	int CurrentDay = 1;


	void HandleOnTimerTick();

	// ~ IPriorityListInterface
	virtual TMap<int, TWeakObjectPtr<UObject>>& GetPriorityItems() override final;
	virtual int& GetHighestPriority() override final;
	virtual void OnPriorityItemChanged(UObject* Item) override;
	virtual void OnPriorityItemRemoved(UObject* Item, bool bReplaced) override;
	virtual void OnPriorityItemCleared() override;
	// ~ End of IPriorityListInterface

private:

	UPROPERTY()
	TMap<int, TWeakObjectPtr<UObject>> _PriorityItems;
	int _HighestPriority = 0;

};


// Module Macros
#undef REN_API

