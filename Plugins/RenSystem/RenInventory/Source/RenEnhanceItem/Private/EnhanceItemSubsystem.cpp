// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EnhanceItemSubsystem.h"

// Engine Headers
#include "Engine/AssetManager.h"
#include "InstancedStruct.h"

// Project Headers
#include "RenAsset/Public/AssetMetadata.h"
#include "RenAsset/Public/Inventory/Asset/Category/EnhanceableAsset.h"
#include "RenAsset/Public/Inventory/Asset/Type/EnhanceAsset.h"
#include "RenAsset/Public/Inventory/Type/InventoryAssetQuantity.h"

#include "RCoreDelegate/Public/LatentDelegates.h"
#include "RCoreEnhance/Public/EnhanceLibrary.h"
#include "RCoreExchange/Public/ExchangeInterface.h"
#include "RCoreExchange/Public/ExchangeRule.h"
#include "RCoreInventory/Public/InventoryRecord.h"
#include "RCoreLibrary/Public/AssetManagerUtils.h"
#include "RCoreLibrary/Public/LogMacro.h"

#include "RenInventory/Public/InventoryPrimaryAsset.h"
#include "RenInventory/Public/InventorySubsystem.h"



void UEnhanceItemSubsystem::LevelUpItem(FName ContainerId, FName TargetId, FName MaterialId)
{
	/*if (TargetId == MaterialId)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Item and enhance are the same"));
		OnItemLevelUp.Broadcast(TargetId, false);
		return;
	}

	UInventorySubsystem* Subsystem = InventorySubsystem.Get();
	if (!IsValid(Subsystem) || !IsValid(AssetManager))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("InventorySubsystem or AssetManager is invalid"));
		OnItemLevelUp.Broadcast(TargetId, false);
		return;
	}

	const FInventoryRecord* TargetRecord = Subsystem->GetItemRecord(ContainerId, TargetId);
	if (!TargetRecord)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Item record is invalid"));
		OnItemLevelUp.Broadcast(TargetId, false);
		return;
	}

	const FInventoryRecord* MaterialRecord = Subsystem->GetItemRecord(ContainerId, MaterialId);
	if (!MaterialRecord)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Enhance record is invalid"));
		OnItemLevelUp.Broadcast(TargetId, false);
		return;
	}

	FEnhanceRecord TargetEnhance = TargetRecord->EnhanceRecord;
	TWeakObjectPtr<UEnhanceItemSubsystem> WeakThis(this);
	
	TFunction<void(bool, UObject*, UObject*)> AsyncCallback = [WeakThis, ContainerId, TargetId, TargetEnhance, MaterialId](bool bSuccess, UObject* TargetObject, UObject* MaterialObject)
	{
		UEnhanceableAsset* TargetAsset = Cast<UEnhanceableAsset>(TargetObject);
		UInventoryAsset* MaterialAsset = Cast<UInventoryAsset>(MaterialObject);
		UEnhanceItemSubsystem* Owner = WeakThis.Get();

		if (!IsValid(TargetAsset) || !IsValid(MaterialAsset) || !IsValid(Owner))
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

		if (!TargetAsset->EnhanceRules.ContainsAny(MaterialAsset->GetPrimaryAssetId(), ItemType))
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

		bool bResult = Owner->HandleLevelUp(ContainerId, TargetId, TargetEnhance, TargetAsset, MaterialId, Point);
		Owner->OnItemLevelUp.Broadcast(TargetId, bResult);
	};

	FPrimaryAssetId TargetAssetId = InventoryPrimaryAsset::GetPrimaryAssetId(TargetRecord->ItemId);
	FPrimaryAssetId MaterialAssetId = InventoryPrimaryAsset::GetPrimaryAssetId(MaterialRecord->ItemId);

	AssetManagerUtils::LoadPrimaryAssets(this, AssetManager, AsyncCallback, TargetAssetId, MaterialAssetId);*/
}

void UEnhanceItemSubsystem::RankUpItem(FName ContainerId, FName TargetId)
{
	/*UInventorySubsystem* Subsystem = InventorySubsystem.Get();
	if (!IsValid(Subsystem))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("InventorySubsystem is invalid"));
		OnItemRankUp.Broadcast(TargetId, false);
		return;
	}

	const FInventoryRecord* TargetRecord = Subsystem->GetItemRecord(ContainerId, TargetId);
	if (!TargetRecord)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Item record is invalid"));
		OnItemRankUp.Broadcast(TargetId, false);
		return;
	}

	FEnhanceRecord TargetEnhance = TargetRecord->EnhanceRecord;
	TWeakObjectPtr<UEnhanceItemSubsystem> WeakThis(this);

	TFunction<void(FPrimaryAssetId, UObject*)> AsyncCallback = [WeakThis, ContainerId, TargetId, TargetEnhance](FPrimaryAssetId AssetId, UObject* TargetObject)
	{
		UEnhanceableAsset* TargetAsset = Cast<UEnhanceableAsset>(TargetObject);
		UEnhanceItemSubsystem* Owner = WeakThis.Get();

		if (!IsValid(TargetAsset) || !IsValid(Owner))
		{
			Owner->OnItemRankUp.Broadcast(TargetId, false);
			return;
		}

		int NextRank = TargetEnhance.Rank;
		const TArray<FExchangeRule>& RankingRules = TargetAsset->RankingRules;
		if (!RankingRules.IsValidIndex(NextRank))
		{
			Owner->OnItemRankUp.Broadcast(TargetId, false);
			return;
		}

		const FExchangeRule& CostItem = RankingRules[NextRank];
		const TMap<FPrimaryAssetId, int>& ItemQuantities = CostItem.RequiredAssets;

		bool bResult = Owner->HandleRankUp(ContainerId, TargetId, TargetEnhance, TargetAsset, ItemQuantities);
		Owner->OnItemRankUp.Broadcast(TargetId, bResult);
	};

	FPrimaryAssetId TargetAssetId = InventoryPrimaryAsset::GetPrimaryAssetId(TargetRecord->ItemId);
	AssetManagerUtils::LoadPrimaryAsset(this, AssetManager, TargetAssetId, AsyncCallback);*/
}


bool UEnhanceItemSubsystem::HandleLevelUp(FName ContainerId, FName EnhanceableId, FEnhanceRecord EnhanceableRecord, UEnhanceableAsset* EnhanceableAsset, FName EnhanceId, int EnhancePoint)
{
	/*UInventorySubsystem* Subsystem = InventorySubsystem.Get();
	if (!IsValid(Subsystem) || !IsValid(EnhanceableAsset))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("InventorySubsystem is invalid"));
		return false;
	}

	int CurrentExperience = EnhanceableRecord.Experience;
	int CurrentLevel = EnhanceableRecord.Level;
	int CurrentRank = EnhanceableRecord.Rank;

	int ExperienceInterval = EnhanceableAsset->GetExperienceInterval(CurrentLevel);
	int LevelInterval = EnhanceableAsset->GetLevelInterval(CurrentRank);
	int MaxLevel = EnhanceableAsset->MaxLevel;

	int NewExperience = 0;
	int NewLevel = 0;

	bool bCanLevelUp = UEnhanceLibrary::CalculateLevelUp(EnhancePoint, CurrentExperience, CurrentLevel, CurrentRank, ExperienceInterval, LevelInterval, MaxLevel, NewExperience, NewLevel);
	if (!bCanLevelUp)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Level up cancelled"));
		return false;
	}

	if (!Subsystem->RemoveItem(ContainerId, EnhanceId, 1))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Failed to remove enhance record"));
		return false;
	}

	bool bResult = Subsystem->UpdateItem(ContainerId, EnhanceableId,
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

	PRINT_INFO(LogTemp, 1.0f, TEXT("Level up successful"));*/
	return true;
}

bool UEnhanceItemSubsystem::HandleRankUp(FName ContainerId, FName TargetId, FEnhanceRecord TargetEnhance, UEnhanceableAsset* TargetAsset, const TMap<FPrimaryAssetId, int>& ItemQuantities)
{
	/*UInventorySubsystem* Subsystem = InventorySubsystem.Get();
	if (!IsValid(Subsystem) || !IsValid(TargetAsset))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("InventorySubsystem, TargetAsset is invalid"));
		return false;
	}

	if (!Subsystem->ContainsItems(ContainerId, ItemQuantities))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Not enough rank costs"));
		return false;
	}
	
	int CurrentExperience = TargetEnhance.Experience;
	int CurrentLevel = TargetEnhance.Level;
	int CurrentRank = TargetEnhance.Rank;

	int LevelInterval = TargetAsset->GetLevelInterval(CurrentRank);
	int MaxRank = TargetAsset->MaxRank;

	bool bCanRankUp = UEnhanceLibrary::CanRankUp(CurrentExperience, CurrentLevel, CurrentRank, LevelInterval, MaxRank);
	if (!bCanRankUp)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Cannot rank up"));
		return false;
	}

	if (!Subsystem->RemoveItems(ContainerId, ItemQuantities))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Failed to remove rank costs"));
		return false;
	}

	bool bResult = Subsystem->UpdateItem(ContainerId, TargetId,
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

	PRINT_INFO(LogTemp, 1.0f, TEXT("Rank up successful"));*/
	return true;
}

void UEnhanceItemSubsystem::CanRankUp(FName ContainerId, FName TargetId, TFunction<void(bool)> Callback)
{
	/*UInventorySubsystem* Subsystem = InventorySubsystem.Get();
	if (!IsValid(Subsystem))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("InventorySubsystem is invalid"));
		Callback(false);
		return;
	}

	const FInventoryRecord* TargetRecord = Subsystem->GetItemRecord(ContainerId, TargetId);
	if (!TargetRecord)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Item record is invalid"));
		Callback(false);
		return;
	}

	FEnhanceRecord TargetEnhance = TargetRecord->EnhanceRecord;

	TFunction<void(FPrimaryAssetId, UObject*)> AsyncCallback = [TargetEnhance, InCallback = MoveTemp(Callback)](FPrimaryAssetId AssetId, UObject* TargetObject)
	{
		UEnhanceableAsset* TargetAsset = Cast<UEnhanceableAsset>(TargetObject);
		if (!IsValid(TargetAsset))
		{
			InCallback(false);
			return;
		}

		int CurrentExperience = TargetEnhance.Experience;
		int CurrentLevel = TargetEnhance.Level;
		int CurrentRank = TargetEnhance.Rank;

		int LevelInterval = TargetAsset->GetLevelInterval(CurrentRank);
		int MaxRank = TargetAsset->MaxRank;

		bool bResult = UEnhanceLibrary::CanRankUp(CurrentExperience, CurrentLevel, CurrentRank, LevelInterval, MaxRank);
		InCallback(bResult);
	};

	FPrimaryAssetId TargetAssetId = InventoryPrimaryAsset::GetPrimaryAssetId(TargetRecord->ItemId);
	AssetManagerUtils::LoadPrimaryAsset(this, AssetManager, TargetAssetId, AsyncCallback);*/
}


void UEnhanceItemSubsystem::HandleGameInitialized()
{
	/*FLatentDelegates::OnPreGameInitialized.RemoveAll(this);

	UInventorySubsystem* Subsystem = GetGameInstance()->GetSubsystem<UInventorySubsystem>();
	if (!IsValid(Subsystem))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("InventorySubsystem is invalid"));
		return;
	}

	InventorySubsystem = Subsystem;
	AssetManager = UAssetManager::GetIfInitialized();*/
}


bool UEnhanceItemSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UEnhanceItemSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogTemp, TEXT("EnhanceItemSubsystem Initialized"));

	if (!FLatentDelegates::OnPreGameInitialized.IsBoundToObject(this))
	{
		FLatentDelegates::OnPreGameInitialized.AddUObject(this, &UEnhanceItemSubsystem::HandleGameInitialized);
	}
}

void UEnhanceItemSubsystem::Deinitialize()
{
	FLatentDelegates::OnPreGameInitialized.RemoveAll(this);

	InventorySubsystem.Reset();

	LOG_WARNING(LogTemp, TEXT("EnhanceItemSubsystem Deinitialized"));
	Super::Deinitialize();
}
