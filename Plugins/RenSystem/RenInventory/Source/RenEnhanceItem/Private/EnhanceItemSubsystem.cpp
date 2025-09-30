// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EnhanceItemSubsystem.h"

// Engine Headers
#include "Engine/AssetManager.h"
#include "InstancedStruct.h"

// Project Headers
#include "RCoreDelegate/Public/LatentDelegates.h"

#include "RCoreEnhance/Public/EnhanceProviderInterface.h"
#include "RCoreEnhance/Public/EnhanceLibrary.h"

#include "RCoreExchange/Public/ExchangeInterface.h"
#include "RCoreExchange/Public/ExchangeRule.h"

#include "RCoreInventory/Public/InventoryRecord.h"

#include "RCoreLibrary/Public/AssetManagerUtils.h"
#include "RCoreLibrary/Public/LogMacro.h"

#include "RenInventory/Public/Asset/Category/EnhanceableAsset.h"
#include "RenInventory/Public/Asset/Type/EnhanceAsset.h"

#include "RenInventory/Public/InventoryPrimaryAsset.h"
#include "RenInventory/Public/InventorySubsystem.h"



void UEnhanceItemSubsystem::LevelUpItem(FName ContainerId, const FPrimaryAssetId& TargetAssetId, FName TargetId, const FPrimaryAssetId& MaterialAssetId, FName MaterialId)
{
	if (TargetId == MaterialId || TargetAssetId == MaterialAssetId)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("TargetId is the same as MaterialId"));
		OnItemLevelUp.Broadcast(TargetId, false);
		return;
	}

	UInventorySubsystem* Subsystem = InventorySubsystem.Get();
	if (!IsValid(Subsystem) || !IsValid(AssetManager))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("InventorySubsystem, AssetManager is invalid"));
		OnItemLevelUp.Broadcast(TargetId, false);
		return;
	}

	const FInventoryRecord* TargetRecord = Subsystem->GetRecordById(ContainerId, TargetAssetId, TargetId);
	const FInventoryRecord* MaterialRecord = Subsystem->GetRecordById(ContainerId, MaterialAssetId, MaterialId);
	if (!TargetRecord || !MaterialRecord)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("TargetRecord, MaterialRecord is invalid"));
		OnItemLevelUp.Broadcast(TargetId, false);
		return;
	}

	FEnhanceRecord TargetEnhance = TargetRecord->EnhanceRecord;
	TWeakObjectPtr<UEnhanceItemSubsystem> WeakThis(this);
	
	TFunction<void(bool, UObject*, UObject*)> AsyncCallback = [WeakThis, ContainerId, TargetAssetId, TargetId, TargetEnhance, MaterialAssetId, MaterialId](bool bSuccess, UObject* TargetObject, UObject* MaterialObject)
	{
		IEnhanceProviderInterface* EnhanceProvider = Cast<IEnhanceProviderInterface>(TargetObject);
		UInventoryAsset* MaterialAsset = Cast<UInventoryAsset>(MaterialObject);
		UEnhanceItemSubsystem* Owner = WeakThis.Get();

		if (!EnhanceProvider || !IsValid(MaterialAsset) || !IsValid(Owner))
		{
			Owner->OnItemLevelUp.Broadcast(TargetId, false);
			return;
		}

		UEnum* Enum = StaticEnum<EInventoryItemType>();
		FName ItemType = Enum->GetNameByValue(static_cast<int64>(MaterialAsset->ItemType));
		if (ItemType == NAME_None)
		{
			Owner->OnItemLevelUp.Broadcast(TargetId, false);
			return;
		}

		const FExchangeRule& EnhanceRule = EnhanceProvider->GetEnhanceRules();
		int Quantity = EnhanceRule.FindAny(MaterialAssetId, ItemType);
		if (Quantity <= 0)
		{
			Owner->OnItemLevelUp.Broadcast(TargetId, false);
			return;
		}

		if (!EnhanceRule.ContainsAny(MaterialAssetId, ItemType))
		{
			Owner->OnItemLevelUp.Broadcast(TargetId, false);
			return;
		}

		IExchangeInterface* ExchangeInterface = Cast<IExchangeInterface>(MaterialAsset);
		if (!ExchangeInterface)
		{
			Owner->OnItemLevelUp.Broadcast(TargetId, false);
			return;
		}

		FInstancedStruct Context = FInstancedStruct::Make(TargetEnhance);
		int Point = ExchangeInterface->GetExchangedNumber(Context);
		if (Point <= 0)
		{
			Owner->OnItemLevelUp.Broadcast(TargetId, false);
			return;
		}

		bool bResult = Owner->HandleLevelUp(ContainerId, TargetAssetId, TargetId, TargetEnhance, EnhanceProvider, MaterialAssetId, MaterialId, Quantity, Point);
		Owner->OnItemLevelUp.Broadcast(TargetId, bResult);
	};

	AssetManagerUtils::LoadPrimaryAssets(this, AssetManager, AsyncCallback, TargetAssetId, MaterialAssetId);
}

void UEnhanceItemSubsystem::RankUpItem(FName ContainerId, const FPrimaryAssetId& TargetAssetId, FName TargetId)
{
	UInventorySubsystem* Inventory = InventorySubsystem.Get();
	if (!IsValid(Inventory))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("InventorySubsystem is invalid"));
		OnItemRankUp.Broadcast(TargetId, false);
		return;
	}

	const FInventoryRecord* TargetRecord = Inventory->GetRecordById(ContainerId, TargetAssetId, TargetId);
	if (!TargetRecord)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("TargetRecord is invalid"));
		OnItemRankUp.Broadcast(TargetId, false);
		return;
	}

	FEnhanceRecord TargetEnhance = TargetRecord->EnhanceRecord;
	TWeakObjectPtr<UEnhanceItemSubsystem> WeakThis(this);

	TFunction<void(FPrimaryAssetId, UObject*)> AsyncCallback = [WeakThis, ContainerId, TargetAssetId, TargetId, TargetEnhance](FPrimaryAssetId AssetId, UObject* TargetObject)
	{
		IEnhanceProviderInterface* EnhanceProvider = Cast<IEnhanceProviderInterface>(TargetObject);
		UEnhanceItemSubsystem* Owner = WeakThis.Get();

		if (!EnhanceProvider || !IsValid(Owner))
		{
			Owner->OnItemRankUp.Broadcast(TargetId, false);
			return;
		}

		int NextRank = TargetEnhance.Rank;

		const TArray<FExchangeRule>& RankingRules = EnhanceProvider->GetRankingRules();
		if (!RankingRules.IsValidIndex(NextRank))
		{
			Owner->OnItemRankUp.Broadcast(TargetId, false);
			return;
		}

		const FExchangeRule& CostItem = RankingRules[NextRank];
		const TMap<FPrimaryAssetId, int>& ItemQuantities = CostItem.RequiredAssets;

		bool bResult = Owner->HandleRankUp(ContainerId, TargetAssetId, TargetId, TargetEnhance, EnhanceProvider, ItemQuantities);
		Owner->OnItemRankUp.Broadcast(TargetId, bResult);
	};

	AssetManagerUtils::LoadPrimaryAsset(this, AssetManager, TargetAssetId, AsyncCallback);
}

void UEnhanceItemSubsystem::CanRankUp(FName ContainerId, const FPrimaryAssetId& TargetAssetId, FName TargetId, TFunction<void(bool)> Callback)
{
	UInventorySubsystem* Inventory = InventorySubsystem.Get();
	if (!IsValid(Inventory))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("InventorySubsystem is invalid"));
		Callback(false);
		return;
	}

	const FInventoryRecord* TargetRecord = Inventory->GetRecordById(ContainerId, TargetAssetId, TargetId);
	if (!TargetRecord)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Item record is invalid"));
		Callback(false);
		return;
	}

	FEnhanceRecord TargetEnhance = TargetRecord->EnhanceRecord;

	TFunction<void(FPrimaryAssetId, UObject*)> AsyncCallback = [TargetEnhance, InCallback = MoveTemp(Callback)](FPrimaryAssetId AssetId, UObject* TargetObject)
	{
		IEnhanceProviderInterface* EnhanceProvider = Cast<IEnhanceProviderInterface>(TargetObject);
		if (!EnhanceProvider)
		{
			InCallback(false);
			return;
		}

		int CurrentExperience = TargetEnhance.Experience;
		int CurrentLevel = TargetEnhance.Level;
		int CurrentRank = TargetEnhance.Rank;

		int LevelInterval = EnhanceProvider->GetLevelInterval(CurrentRank);
		int MaxRank = EnhanceProvider->GetMaxRank();

		bool bResult = UEnhanceLibrary::CanRankUp(CurrentExperience, CurrentLevel, CurrentRank, LevelInterval, MaxRank);
		InCallback(bResult);
	};

	AssetManagerUtils::LoadPrimaryAsset(this, AssetManager, TargetAssetId, AsyncCallback);
}

bool UEnhanceItemSubsystem::CanRankUp(FName ContainerId, const FPrimaryAssetId& TargetAssetId, FName TargetId, IEnhanceProviderInterface* TargetAsset)
{
	IEnhanceProviderInterface* EnhanceProvider = Cast<IEnhanceProviderInterface>(TargetAsset);
	UInventorySubsystem* Inventory = InventorySubsystem.Get();
	if (!IsValid(Inventory) || !EnhanceProvider)
	{
		return false;
	}

	const FInventoryRecord* TargetRecord = Inventory->GetRecordById(ContainerId, TargetAssetId, TargetId);
	if (!TargetRecord)
	{
		return false;
	}

	const FEnhanceRecord& TargetEnhance = TargetRecord->EnhanceRecord;
	return CanRankUp(TargetEnhance, EnhanceProvider);
}

bool UEnhanceItemSubsystem::CanRankUp(const FEnhanceRecord& EnhanceRecord, IEnhanceProviderInterface* TargetAsset)
{
	IEnhanceProviderInterface* EnhanceProvider = Cast<IEnhanceProviderInterface>(TargetAsset);
	if (!EnhanceProvider)
	{
		return false;
	}

	int CurrentExperience = EnhanceRecord.Experience;
	int CurrentLevel = EnhanceRecord.Level;
	int CurrentRank = EnhanceRecord.Rank;

	int LevelInterval = EnhanceProvider->GetLevelInterval(CurrentRank);
	int MaxRank = EnhanceProvider->GetMaxRank();

	return UEnhanceLibrary::CanRankUp(CurrentExperience, CurrentLevel, CurrentRank, LevelInterval, MaxRank);
}



bool UEnhanceItemSubsystem::HandleLevelUp(FName ContainerId, const FPrimaryAssetId& TargetAssetId, FName TargetId, const FEnhanceRecord& TargetEnhance, IEnhanceProviderInterface* TargetAsset, const FPrimaryAssetId& MaterialAssetId, FName MaterialId, int MaterialQuantity, int Point)
{
	UInventorySubsystem* Inventory = InventorySubsystem.Get();
	if (!IsValid(Inventory) || !TargetAsset)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("InventorySubsystem is invalid"));
		return false;
	}

	int CurrentExperience = TargetEnhance.Experience;
	int CurrentLevel = TargetEnhance.Level;
	int CurrentRank = TargetEnhance.Rank;

	int ExperienceInterval = TargetAsset->GetExperienceInterval(CurrentLevel);
	int LevelInterval = TargetAsset->GetLevelInterval(CurrentRank);
	int MaxLevel = TargetAsset->GetMaxLevel();

	int NewExperience = 0;
	int NewLevel = 0;

	bool bCanLevelUp = UEnhanceLibrary::CalculateLevelUp((Point * MaterialQuantity), CurrentExperience, CurrentLevel, CurrentRank, ExperienceInterval, LevelInterval, MaxLevel, NewExperience, NewLevel);
	if (!bCanLevelUp)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Level up cancelled"));
		return false;
	}

	if (!Inventory->RemoveItemById(ContainerId, MaterialAssetId, MaterialId, MaterialQuantity))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Failed to remove enhance record"));
		return false;
	}

	bool bResult = Inventory->UpdateItemById(ContainerId, TargetAssetId, TargetId,
		[NewExperience, NewLevel](FInventoryRecord* Record)
		{
			if (!Record) return false;
			Record->EnhanceRecord.Experience = NewExperience;
			Record->EnhanceRecord.Level = NewLevel;
			return true;
		}
	);

	if (!bResult)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Failed to update enhanceable record"));
		return false;
	}

	PRINT_INFO(LogTemp, 1.0f, TEXT("Level up successful"));
	return true;
}

bool UEnhanceItemSubsystem::HandleRankUp(FName ContainerId, const FPrimaryAssetId& TargetAssetId, FName TargetId, const FEnhanceRecord& TargetEnhance, IEnhanceProviderInterface* TargetAsset, const TMap<FPrimaryAssetId, int>& MaterialQuantities)
{
	UInventorySubsystem* Inventory = InventorySubsystem.Get();
	if (!IsValid(Inventory) || !TargetAsset)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("InventorySubsystem, TargetAsset is invalid"));
		return false;
	}

	if (!Inventory->ContainsItems(ContainerId, MaterialQuantities))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Not enough rank costs"));
		return false;
	}
	
	int CurrentExperience = TargetEnhance.Experience;
	int CurrentLevel = TargetEnhance.Level;
	int CurrentRank = TargetEnhance.Rank;

	int LevelInterval = TargetAsset->GetLevelInterval(CurrentRank);
	int MaxRank = TargetAsset->GetMaxRank();

	bool bCanRankUp = UEnhanceLibrary::CanRankUp(CurrentExperience, CurrentLevel, CurrentRank, LevelInterval, MaxRank);
	if (!bCanRankUp)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Cannot rank up"));
		return false;
	}

	if (!Inventory->RemoveItems(ContainerId, MaterialQuantities))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Failed to remove rank costs"));
		return false;
	}

	bool bResult = Inventory->UpdateItemById(ContainerId, TargetAssetId, TargetId,
		[](FInventoryRecord* Record)
		{
			if (!Record) return false;
			Record->EnhanceRecord.Rank++;
			return true;
		}
	);

	if (!bResult)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Failed to update enhanceable record"));
		return false;
	}

	PRINT_INFO(LogTemp, 1.0f, TEXT("Rank up successful"));
	return true;
}

void UEnhanceItemSubsystem::HandleGameInitialized()
{
	FLatentDelegates::OnPreGameInitialized.RemoveAll(this);

	UInventorySubsystem* Inventory = GetGameInstance()->GetSubsystem<UInventorySubsystem>();
	if (!IsValid(Inventory))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("InventorySubsystem is invalid"));
		return;
	}

	InventorySubsystem = Inventory;
	AssetManager = UAssetManager::GetIfInitialized();
}



bool UEnhanceItemSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UEnhanceItemSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogTemp, TEXT("EnhanceItemSubsystem initialized"));

	if (!FLatentDelegates::OnPreGameInitialized.IsBoundToObject(this))
	{
		FLatentDelegates::OnPreGameInitialized.AddUObject(this, &UEnhanceItemSubsystem::HandleGameInitialized);
	}
}

void UEnhanceItemSubsystem::Deinitialize()
{
	FLatentDelegates::OnPreGameInitialized.RemoveAll(this);

	InventorySubsystem.Reset();
	AssetManager = nullptr;

	LOG_WARNING(LogTemp, TEXT("EnhanceItemSubsystem deinitialized"));
	Super::Deinitialize();
}

