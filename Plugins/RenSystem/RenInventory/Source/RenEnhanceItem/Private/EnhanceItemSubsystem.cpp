// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EnhanceItemSubsystem.h"

// Engine Headers
#include "Engine/AssetManager.h"
#include "InstancedStruct.h"

// Project Headers
#include "RCoreDelegate/Public/LatentDelegates.h"

#include "RCoreEnhance/Public/EnhanceLibrary.h"
#include "RCoreEnhance/Public/EnhanceProviderInterface.h"
#include "RCoreExchange/Public/ExchangeRule.h"

#include "RCoreExchange/Public/ExchangeProviderInterface.h"
#include "RCoreExchange/Public/ExchangeRule.h"

#include "RCoreInventory/Public/InventoryAsset.h"
#include "RCoreInventory/Public/InventoryRecord.h"

#include "RCoreLibrary/Public/AssetManagerUtils.h"
#include "RCoreLibrary/Public/LogCategory.h"
#include "RCoreLibrary/Public/LogMacro.h"

#include "RenInventory/Public/InventoryPrimaryAsset.h"
#include "RenInventory/Public/InventorySubsystem.h"



void UEnhanceItemSubsystem::LevelUpItem(FName ContainerId, const FPrimaryAssetId& TargetAssetId, FName TargetId, const FPrimaryAssetId& MaterialAssetId, FName MaterialId)
{
	bool bRecordIdValid = TargetId == MaterialId || TargetAssetId == MaterialAssetId;
	bool bAssetIdValid = InventoryPrimaryAsset::IsValid(TargetAssetId) && InventoryPrimaryAsset::IsValid(MaterialAssetId);
	if (bRecordIdValid || !bAssetIdValid)
	{
		PRINT_ERROR(LogEnhanceItem, 1.0f, TEXT("RecordId, AssetId is invalid"));
		OnItemLevelUp.Broadcast(TargetId, false);
		return;
	}

	UInventorySubsystem* Inventory = InventorySubsystem.Get();
	if (!IsValid(Inventory) || !IsValid(AssetManager))
	{
		PRINT_ERROR(LogEnhanceItem, 1.0f, TEXT("InventorySubsystem, AssetManager is invalid"));
		OnItemLevelUp.Broadcast(TargetId, false);
		return;
	}

	const FInventoryRecord* TargetRecord = Inventory->GetRecordById(ContainerId, TargetAssetId, TargetId);
	const FInventoryRecord* MaterialRecord = Inventory->GetRecordById(ContainerId, MaterialAssetId, MaterialId);
	if (!TargetRecord || !MaterialRecord)
	{
		PRINT_ERROR(LogEnhanceItem, 1.0f, TEXT("TargetRecord, MaterialRecord is invalid"));
		OnItemLevelUp.Broadcast(TargetId, false);
		return;
	}

	FEnhanceRecord TargetEnhance = TargetRecord->Enhancement;
	TWeakObjectPtr<UEnhanceItemSubsystem> WeakThis(this);
	
	TFunction<void(bool, UObject*, UObject*)> AsyncCallback = [WeakThis, ContainerId, TargetAssetId, TargetId, TargetEnhance, MaterialAssetId, MaterialId](bool bSuccess, UObject* TargetObject, UObject* MaterialObject)
		{
			UEnhanceItemSubsystem* Owner = WeakThis.Get();
			if (IsValid(Owner))
			{
				bool bResult = Owner->HandleLevelUp(ContainerId, TargetAssetId, TargetId, TargetEnhance, TargetObject, MaterialAssetId, MaterialId, MaterialObject);
				Owner->OnItemLevelUp.Broadcast(TargetId, bResult);
			}
		};

	AssetManagerUtils::LoadPrimaryAssetsArgs(this, AssetManager, MoveTemp(AsyncCallback), TargetAssetId, MaterialAssetId);
}

void UEnhanceItemSubsystem::RankUpItem(FName ContainerId, const FPrimaryAssetId& TargetAssetId, FName TargetId)
{
	UInventorySubsystem* Inventory = InventorySubsystem.Get();
	if (!InventoryPrimaryAsset::IsValid(TargetAssetId) || !IsValid(Inventory))
	{
		PRINT_ERROR(LogEnhanceItem, 1.0f, TEXT("TargetAssetId, InventorySubsystem is invalid"));
		OnItemRankUp.Broadcast(TargetId, false);
		return;
	}

	const FInventoryRecord* TargetRecord = Inventory->GetRecordById(ContainerId, TargetAssetId, TargetId);
	if (!TargetRecord)
	{
		PRINT_ERROR(LogEnhanceItem, 1.0f, TEXT("TargetRecord is invalid"));
		OnItemRankUp.Broadcast(TargetId, false);
		return;
	}

	FEnhanceRecord TargetEnhance = TargetRecord->Enhancement;
	TWeakObjectPtr<UEnhanceItemSubsystem> WeakThis(this);

	TFunction<void(bool, UObject*)> AsyncCallback = [WeakThis, ContainerId, TargetAssetId, TargetId, TargetEnhance](bool, UObject* TargetObject)
		{
			UEnhanceItemSubsystem* Owner = WeakThis.Get();
			if (IsValid(Owner))
			{
				bool bResult = Owner->HandleRankUp(ContainerId, TargetAssetId, TargetId, TargetEnhance, TargetObject);
				Owner->OnItemLevelUp.Broadcast(TargetId, bResult);
			}
		};

	AssetManagerUtils::LoadPrimaryAsset(this, AssetManager, TargetAssetId, MoveTemp(AsyncCallback));
}

void UEnhanceItemSubsystem::CanRankUp(FName ContainerId, const FPrimaryAssetId& TargetAssetId, FName TargetId, TFunction<void(bool)> Callback)
{
	UInventorySubsystem* Inventory = InventorySubsystem.Get();
	if (!IsValid(Inventory))
	{
		PRINT_ERROR(LogEnhanceItem, 1.0f, TEXT("InventorySubsystem is invalid"));
		Callback(false);
		return;
	}

	const FInventoryRecord* TargetRecord = Inventory->GetRecordById(ContainerId, TargetAssetId, TargetId);
	if (!TargetRecord)
	{
		PRINT_ERROR(LogEnhanceItem, 1.0f, TEXT("Item record is invalid"));
		Callback(false);
		return;
	}

	FEnhanceRecord TargetEnhance = TargetRecord->Enhancement;

	TFunction<void(bool, UObject*)> AsyncCallback = [TargetEnhance, InCallback = MoveTemp(Callback)](bool, UObject* TargetObject)
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

	AssetManagerUtils::LoadPrimaryAsset(this, AssetManager, TargetAssetId, MoveTemp(AsyncCallback));
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

	const FEnhanceRecord& TargetEnhance = TargetRecord->Enhancement;
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



bool UEnhanceItemSubsystem::HandleLevelUp(FName ContainerId, const FPrimaryAssetId& TargetAssetId, FName TargetId, const FEnhanceRecord& TargetEnhancement, UObject* TargetObject, const FPrimaryAssetId& MaterialAssetId, FName MaterialId, UObject* MaterialObject)
{
	UInventorySubsystem* Inventory = InventorySubsystem.Get();
	IEnhanceProviderInterface* EnhanceProvider = Cast<IEnhanceProviderInterface>(TargetObject);
	IExchangeProviderInterface* ExchangeProvider = Cast<IExchangeProviderInterface>(MaterialObject);
	UInventoryAsset* MaterialAsset = Cast<UInventoryAsset>(MaterialObject);
	if (!IsValid(Inventory) || !EnhanceProvider || !ExchangeProvider || !IsValid(MaterialAsset))
	{
		PRINT_ERROR(LogEnhanceItem, 1.0f, TEXT("Inventory, EnhanceProvider, ExchangeProvider, MaterialAsset is invalid"));
		return false;
	}

	UEnum* Enum = StaticEnum<EInventoryItemType>();
	FName ItemType = Enum->GetNameByValue(static_cast<int64>(MaterialAsset->Type));
	if (!ItemType.IsValid())
	{
		PRINT_ERROR(LogEnhanceItem, 1.0f, TEXT("ItemType is invalid"));
		return false;
	}

	const FExchangeRule& EnhanceRule = EnhanceProvider->GetEnhanceRules();
	int Quantity = EnhanceRule.FindAny(MaterialAssetId, ItemType);
	if (Quantity <= 0)
	{
		PRINT_ERROR(LogEnhanceItem, 1.0f, TEXT("Quantity is invalid"));
		return false;
	}

	bool bHasMaterial = Inventory->ContainsItemById(ContainerId, MaterialAssetId, MaterialId, Quantity);
	if (!bHasMaterial)
	{
		PRINT_ERROR(LogEnhanceItem, 1.0f, TEXT("Not enough material"));
		return false;
	}

	FInstancedStruct Context = FInstancedStruct::Make(TargetEnhancement);
	int Point = ExchangeProvider->GetExchangedNumber(Context);
	if (Point <= 0)
	{
		PRINT_ERROR(LogEnhanceItem, 1.0f, TEXT("Point is invalid"));
		return false;
	}

	int TotalPoint = Point * Quantity;

	int CurrentLevel = TargetEnhancement.Level;
	int CurrentRank = TargetEnhancement.Rank;

	int ExperienceInterval = EnhanceProvider->GetExperienceInterval(CurrentLevel);
	int LevelInterval = EnhanceProvider->GetLevelInterval(CurrentRank);
	int MaxLevel = EnhanceProvider->GetMaxLevel();

	int NewExperience = 0;
	int NewLevel = 0;

	bool bCanLevelUp = UEnhanceLibrary::CalculateLevelUp(TotalPoint, TargetEnhancement, ExperienceInterval, LevelInterval, MaxLevel, NewExperience, NewLevel);
	if (!bCanLevelUp)
	{
		PRINT_ERROR(LogEnhanceItem, 1.0f, TEXT("Level up cancelled"));
		return false;
	}

	if (!Inventory->RemoveItemById(ContainerId, MaterialAssetId, MaterialId, Quantity))
	{
		PRINT_ERROR(LogEnhanceItem, 1.0f, TEXT("Failed to remove enhance record"));
		return false;
	}

	bool bResult = Inventory->UpdateItemById(ContainerId, TargetAssetId, TargetId,
		[NewExperience, NewLevel](FInventoryRecord* Record)
		{
			if (!Record)
			{
				return false;
			}
			Record->Enhancement.Experience = NewExperience;
			Record->Enhancement.Level = NewLevel;
			return true;
		}
	);

	if (!bResult)
	{
		PRINT_ERROR(LogEnhanceItem, 1.0f, TEXT("Failed to update enhanceable record"));
		return false;
	}

	PRINT_INFO(LogEnhanceItem, 1.0f, TEXT("Level up successful"));
	return true;
}

bool UEnhanceItemSubsystem::HandleRankUp(FName ContainerId, const FPrimaryAssetId& TargetAssetId, FName TargetId, const FEnhanceRecord& TargetEnhancement, UObject* TargetObject)
{
	UInventorySubsystem* Inventory = InventorySubsystem.Get();
	IEnhanceProviderInterface* EnhanceProvider = Cast<IEnhanceProviderInterface>(TargetObject);
	if (!IsValid(Inventory) || !EnhanceProvider)
	{
		PRINT_ERROR(LogEnhanceItem, 1.0f, TEXT("InventorySubsystem, EnhanceProvider is invalid"));
		return false;
	}

	const TArray<FExchangeRule>& RankingRules = EnhanceProvider->GetRankingRules();

	int NextRank = TargetEnhancement.Rank;
	if (!RankingRules.IsValidIndex(NextRank))
	{
		PRINT_ERROR(LogEnhanceItem, 1.0f, TEXT("Invalid rank"));
		return false;
	}

	const FExchangeRule& CostItem = RankingRules[NextRank];
	const TMap<FPrimaryAssetId, int>& RequiredAssets = CostItem.RequiredAssets;
	if (!Inventory->ContainsItems(ContainerId, RequiredAssets))
	{
		PRINT_ERROR(LogEnhanceItem, 1.0f, TEXT("Not enough rank costs"));
		return false;
	}
	
	int CurrentRank = TargetEnhancement.Rank;

	int LevelInterval = EnhanceProvider->GetLevelInterval(CurrentRank);
	int MaxRank = EnhanceProvider->GetMaxRank();

	bool bCanRankUp = UEnhanceLibrary::CanRankUp(TargetEnhancement, LevelInterval, MaxRank);
	if (!bCanRankUp)
	{
		PRINT_ERROR(LogEnhanceItem, 1.0f, TEXT("Cannot rank up"));
		return false;
	}

	if (!Inventory->RemoveItems(ContainerId, RequiredAssets))
	{
		PRINT_ERROR(LogEnhanceItem, 1.0f, TEXT("Failed to remove rank costs"));
		return false;
	}

	bool bResult = Inventory->UpdateItemById(ContainerId, TargetAssetId, TargetId,
		[](FInventoryRecord* Record)
		{
			if (!Record)
			{
				return false;
			}
			Record->Enhancement.Rank++;
			return true;
		}
	);

	if (!bResult)
	{
		PRINT_ERROR(LogEnhanceItem, 1.0f, TEXT("Failed to update enhanceable record"));
		return false;
	}

	PRINT_INFO(LogEnhanceItem, 1.0f, TEXT("Rank up successful"));
	return true;
}

void UEnhanceItemSubsystem::HandleGameInitialized()
{
	FLatentDelegates::OnPreGameInitialized.RemoveAll(this);

	InventorySubsystem = GetGameInstance()->GetSubsystem<UInventorySubsystem>();
	AssetManager = UAssetManager::GetIfInitialized();
}



bool UEnhanceItemSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UEnhanceItemSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogEnhanceItem, TEXT("Initialized"));

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

	LOG_WARNING(LogEnhanceItem, TEXT("Deinitialized"));
	Super::Deinitialize();
}

