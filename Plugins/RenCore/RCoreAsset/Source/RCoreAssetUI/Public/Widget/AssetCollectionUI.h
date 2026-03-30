// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Blueprint/UserWidget.h"
#include "InstancedStruct.h"

// Generated Headers
#include "AssetCollectionUI.generated.h"

// Module Macros
#define REN_API RCOREASSETUI_API

// Forward Declarations
class UListView;
class UAssetEntry;
class UFilterGroup;
class UFilterCriterion;



/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UAssetCollectionUI : public UUserWidget
{

	GENERATED_BODY()

public:

	DECLARE_DELEGATE_OneParam(FOnSelectionChanged, const UAssetEntry* /* Entry */);
	FOnSelectionChanged OnSelectionChanged;

	DECLARE_DELEGATE(FOnSelectionCleared);
	FOnSelectionCleared OnSelectionCleared;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (ExposeOnSpawn = true))
	FName PrimarySourceId = NAME_None;


	REN_API virtual void InitializeCollection();

	REN_API virtual void DisplayEntries();
	REN_API virtual void ClearEntries(bool bRegenerate);
	REN_API virtual void RefreshEntries();

	REN_API UAssetEntry* GetSelectedEntry();
	template<typename T>
	T* GetSelectedEntry()
	{
		return Cast<T>(GetSelectedEntry());
	}


	REN_API void AddSubDetails(const FPrimaryAssetId& Id, const FInstancedStruct& Detail);
	REN_API void RemoveSubDetails(const FPrimaryAssetId& Id);
	REN_API void ClearSubDetails();


	REN_API UFilterCriterion* GetCriterionByName(FName Name) const;

	template<typename T>
	T* GetCriterionByName(FName Name)
	{
		return Cast<T>(GetCriterionByName(Name));
	}

protected:

	UPROPERTY(EditAnywhere)
	bool bAutoRefresh = false;

	UPROPERTY(EditAnywhere)
	bool bAutoClearSelection = false;

	UPROPERTY(EditAnywhere)
	bool bAutoSelectAfterRefresh = false;

	UPROPERTY()
	TMap<FPrimaryAssetId, FInstancedStruct> SubDetails;

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite)
	TObjectPtr<UFilterGroup> FilterRule = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UListView> EntryList = nullptr;

	UPROPERTY()
	TArray<TObjectPtr<UAssetEntry>> EntryPool;


	REN_API const UFilterCriterion* GetFilterRoot() const;


	REN_API virtual void AutoSelectCaching();
	REN_API virtual bool AutoSelectCondition(UAssetEntry* Item) const;


	REN_API void AddEntry(const FPrimaryAssetId& AssetId, UAssetEntry* Entry);
	REN_API void ReturnEntryToPool(UAssetEntry* Item);
	REN_API UAssetEntry* GetEntryFromPool(const TSubclassOf<UAssetEntry>& EntryClass);

	template<typename T>
	T* GetEntryFromPool()
	{
		return Cast<T>(GetEntryFromPool(T::StaticClass()));
	}


	void HandleOnItemSelectionChanged(UObject* Object);

	// ~ UUserWidget
	REN_API virtual void NativeConstruct() override;
	REN_API virtual void NativeDestruct() override;
	// ~ End of UUserWidget

private:

	FPrimaryAssetId _SelectedAssetId;

};


// Module Macros
#undef REN_API

