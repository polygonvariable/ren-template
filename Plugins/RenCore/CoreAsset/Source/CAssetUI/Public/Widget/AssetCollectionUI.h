// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Blueprint/UserWidget.h"
#include "StructUtils/InstancedStruct.h"

// Project Headers
#include "FilterGroup.h"

// Generated Headers
#include "AssetCollectionUI.generated.h"

// Forward Declarations
class UListView;
class UAssetEntry;


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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true))
	FName PrimarySourceId = NAME_None;

	UPROPERTY(EditAnywhere)
	FFilterGroup FilterRule;


	UFUNCTION(BlueprintCallable)
	CASSETUI_API virtual void InitializeCollection();

	UFUNCTION(BlueprintCallable)
	CASSETUI_API virtual void DisplayEntries();
	CASSETUI_API virtual void ClearEntries(bool bRegenerate);
	CASSETUI_API virtual void RefreshEntries();

	CASSETUI_API UAssetEntry* GetSelectedEntry();
	template<typename T>
	T* GetSelectedEntry()
	{
		return Cast<T>(GetSelectedEntry());
	}

	CASSETUI_API void AddSubDetails(const FPrimaryAssetId& Id, const FInstancedStruct& Detail);
	CASSETUI_API void RemoveSubDetails(const FPrimaryAssetId& Id);
	CASSETUI_API void ClearSubDetails();

protected:

	UPROPERTY(EditAnywhere)
	bool bAutoRefresh = false;

	UPROPERTY(EditAnywhere)
	bool bAutoClearSelection = false;

	UPROPERTY(EditAnywhere)
	bool bAutoSelectAfterRefresh = false;

	UPROPERTY()
	TMap<FPrimaryAssetId, FInstancedStruct> SubDetails;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UListView> EntryList = nullptr;

	UPROPERTY()
	TArray<TObjectPtr<UAssetEntry>> EntryPool;


	CASSETUI_API virtual void AutoSelectCaching();
	CASSETUI_API virtual bool AutoSelectCondition(UAssetEntry* Item) const;

	CASSETUI_API void AddEntry(const FPrimaryAssetId& AssetId, UAssetEntry* Entry);
	CASSETUI_API void ReturnEntryToPool(UAssetEntry* Item);
	CASSETUI_API UAssetEntry* GetEntryFromPool(const TSubclassOf<UAssetEntry>& EntryClass);

	template<typename T>
	T* GetEntryFromPool()
	{
		return Cast<T>(GetEntryFromPool(T::StaticClass()));
	}

	void HandleOnItemSelectionChanged(UObject* Object);

	// ~ UUserWidget
	CASSETUI_API virtual void NativeConstruct() override;
	CASSETUI_API virtual void NativeDestruct() override;
	// ~ End of UUserWidget

private:

	FPrimaryAssetId _SelectedAssetId;

};

