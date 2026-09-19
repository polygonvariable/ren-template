// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Priority/PriorityListInterface.h"

// Generated Headers
#include "SeasonController.generated.h"

// Forward Declarations
class UMaterialParameterCollectionInstance;
class USeasonCollectionAsset;
class USeasonAsset;
class UEnvironmentSubsystem;
class IClockManagerInterface;


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

	bool AddSeasonCollection(USeasonCollectionAsset* SeasonCollection, int Priority);
	bool RemoveSeasonCollection(int Priority);

#if UE_BUILD_DEVELOPMENT
	RSEASON_API int GetEditorYearLength() const;
	RSEASON_API int GetEditorCurrentDay() const;
	RSEASON_API const USeasonCollectionAsset* GetEditorCurrentSeasonCollection() const;
	RSEASON_API const TMap<int, TWeakObjectPtr<UObject>>& GetEditorSeasonCollectionList() const;
#endif

protected:

	int YearLength = 1;

	UPROPERTY()
	TObjectPtr<UMaterialParameterCollectionInstance>  MPCInstance;

	UPROPERTY()
	TObjectPtr<UEnvironmentSubsystem> EnvironmentSubsystem;

	UPROPERTY()
	TObjectPtr<USeasonCollectionAsset> CurrentCollection;

	IClockManagerInterface* ClockManager;


	// ~ Binding
	void HandleOnDayChanged(int Day);
	// ~ End of Binding

	// ~ IPriorityListInterface
	virtual TMap<int, TWeakObjectPtr<UObject>>& GetPriorityItems() override final;
	virtual void OnPriorityItemChanged(UObject* Item) override;
	virtual void OnPriorityItemRemoved(UObject* Item, bool bReplaced) override;
	virtual void OnPriorityItemCleared() override;
	// ~ End of IPriorityListInterface

private:

	UPROPERTY()
	TMap<int, TWeakObjectPtr<UObject>> _SeasonItems;

};

