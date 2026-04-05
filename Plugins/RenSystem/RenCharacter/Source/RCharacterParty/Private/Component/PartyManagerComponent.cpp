// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Component/PartyManagerComponent.h"

// Engine Headers
#include "GameFramework/Character.h"
#include "UObject/ObjectSaveContext.h"

// Project Headers
#include "Asset/CharacterAsset.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Manager/RAssetManager.inl"
#include "Settings/PartySettings.h"
#include "Storage/PartyStorage.h"
#include "Subsystem/PartySubsystem.h"


UPartyManagerComponent::UPartyManagerComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	bAutoActivate = false;
	SetIsReplicatedByDefault(true);
}

void UPartyManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UPartyManagerComponent::PreSave(FObjectPreSaveContext ObjectSaveContext)
{
	Super::PreSave(ObjectSaveContext);

#if WITH_EDITOR
	PartyOrder.Empty();
	if (SourceType == EAssetQuerySource::Asset)
	{
		for (const FCharacterData& Data : SpawnData)
		{
			PartyOrder.Add(Data.AssetId);
		}
	}
	else
	{
		SpawnData.Empty();
	}
#endif
}


void UPartyManagerComponent::BeginPlay()
{
	AssetManager = Cast<URAssetManager>(UAssetManager::GetIfInitialized());

	PartySubsystem = UPartySubsystem::Get(GetWorld());
	if (IsValid(PartySubsystem))
	{
		PartySubsystem->OnSyncParty.AddUObject(this, &UPartyManagerComponent::SpawnParty);
		PartyStorage = PartySubsystem->GetPartyStorage();
	}

	Super::BeginPlay();
}

void UPartyManagerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (IsValid(PartySubsystem))
	{
		PartySubsystem->OnSyncParty.RemoveAll(this);
	}

	AssetManager = nullptr;
	PartySubsystem = nullptr;
	PartyStorage = nullptr;

	Super::EndPlay(EndPlayReason);
}

//void UPartyManagerComponent::SwitchCharacter(int Index)
//{
//	if (!PartyOrder.IsValidIndex(Index))
//	{
//		return;
//	}
//
//	TObjectPtr<ACharacterBase>* FoundCharacter = PartyCharacters.Find(PartyOrder[Index]);
//	if (!FoundCharacter)
//	{
//		return;
//	}
//
//	ACharacterBase* Character = FoundCharacter->Get();
//	APlayerController* PC = GetControllerWithAuthority();
//	if (!IsValid(Character) || !Character->IsAlive() || !IsValid(PC))
//	{
//		return;
//	}
//
//	PC->Possess(Character);
//}
//
//void UPartyManagerComponent::SpawnParty()
//{
//	if (!IsValid(AssetManager) || !IsValid(PartyStorage))
//	{
//		LOG_ERROR(LogCharacterParty, TEXT("AssetManager, Party Storage is invalid"));
//		return;
//	}
//
//	PartyStorage->GetAllCharacters(PartyOrder);
//
//	TArray<FPrimaryAssetId> CurrentIds;
//	PartyCharacters.GetKeys(CurrentIds);
//
//	for (const FPrimaryAssetId& AssetId : CurrentIds)
//	{
//		if (!PartyOrder.Contains(AssetId))
//		{
//			ACharacterBase* Character = PartyCharacters.FindAndRemoveChecked(AssetId);
//			if (Character)
//			{
//				RemoveCharacter(Character);
//			}
//		}
//	}
//
//	TArray<FPrimaryAssetId> SpawnList;
//	for (const FPrimaryAssetId& AssetId : PartyOrder)
//	{
//		if (!PartyCharacters.Contains(AssetId))
//		{
//			SpawnList.Add(AssetId);
//		}
//	}
//
//	if (SpawnList.Num() == 0)
//	{
//		PossessAliveCharacter();
//		return;
//	}
//
//	const UPartySettings* Settings = UPartySettings::Get();
//
//	AssetManager->CancelFetch(_PartySpawnId);
//
//	_PartySpawnId = FGuid::NewGuid();
//	_PartySpawnLocation = PartyStorage->GetPartyLocation(*GetWorld()->GetMapName());
//
//	const TArray<FName>& AssetBundles = Settings->CharacterBundles;
//	//AssetBundles.Add(TEXT("Character"));
//	//AssetBundles.Add(TEXT("Ability"));
//
//	TFuture<FLatentLoadedAssets<UCharacterAsset>> Future = AssetManager->FetchPrimaryAssets<UCharacterAsset>(_PartySpawnId, SpawnList, AssetBundles);
//	if (!Future.IsValid())
//	{
//		LOG_ERROR(LogCharacterParty, TEXT("Failed to initialize load character assets"));
//		return;
//	}
//
//	TWeakObjectPtr<UPartyManagerComponent> WeakThis(this);
//	Future.Next([WeakThis](const FLatentLoadedAssets<UCharacterAsset>& Result)
//		{
//			UPartyManagerComponent* This = WeakThis.Get();
//			if (IsValid(This) && Result.IsValid())
//			{
//				This->SpawnParty_Internal(Result.Get());
//			}
//		}
//	);
//}
//
//void UPartyManagerComponent::SpawnParty_Internal(const TArray<UCharacterAsset*>& Assets)
//{
//	UWorld* World = GetWorld();
//
//	for (const UCharacterAsset* Asset : Assets)
//	{
//		if (IsValid(Asset))
//		{
//			SpawnCharacter(World, Asset);
//		}
//	}
//
//	PossessAliveCharacter();
//}
//
//
//void UPartyManagerComponent::SpawnCharacter(UWorld* World, const UCharacterAsset* CharacterAsset)
//{
//	UClass* CharacterClass = CharacterAsset->CharacterClass.Get();
//	if (!CharacterClass)
//	{
//		LOG_ERROR(LogCharacterParty, TEXT("CharacterClass is invalid"));
//		return;
//	}
//
//	FTransform SpawnTransform;
//	ACharacterBase* Character = World->SpawnActorDeferred<ACharacterBase>(CharacterClass, SpawnTransform, GetOwner(), nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
//	if (!IsValid(Character))
//	{
//		LOG_ERROR(LogCharacterParty, TEXT("Spawned character is invalid"));
//		return;
//	}
//
//	Character->InitializeCharacter(CharacterAsset);
//	Character->FinishSpawning(SpawnTransform);
//
//	RegisterCharacter(Character);
//
//	PartyCharacters.Add(CharacterAsset->GetPrimaryAssetId(), Character);
//}
//
//void UPartyManagerComponent::RemoveCharacter(ACharacterBase* Character)
//{
//	UnregisterCharacter(Character);
//
//	Character->Destroy();
//}
//
//
//void UPartyManagerComponent::RegisterCharacter(ACharacterBase* Character)
//{
//	Character->OnCharacterDied.AddUObject(this, &UPartyManagerComponent::HandleOnCharacterDied);
//}
//
//void UPartyManagerComponent::UnregisterCharacter(ACharacterBase* Character)
//{
//	Character->OnCharacterDied.RemoveAll(this);
//	Character->DeinitializeCharacter();
//	Character->Destroy();
//}
//
//
//void UPartyManagerComponent::HandleOnCharacterDied()
//{
//	ACharacter* Character = GetAliveCharacter();
//	APlayerController* PC = GetControllerWithAuthority();
//	if (!IsValid(PC) || !IsValid(Character))
//	{
//		LOG_ERROR(LogCharacterParty, TEXT("PC, Character is invalid"));
//		return;
//	}
//
//	PC->Possess(Character);
//}
//
//
//void UPartyManagerComponent::ClearParty()
//{
//	APlayerController* PC = GetControllerWithAuthority();
//	if (!IsValid(PC))
//	{
//		LOG_ERROR(LogCharacterParty, TEXT("PC is invalid"));
//		return;
//	}
//
//	PC->UnPossess();
//}
//

void UPartyManagerComponent::PossessAliveCharacter()
{
	APlayerController* PC = GetControllerWithAuthority();
	ACharacter* Character = GetAliveCharacter();
	if (IsValid(Character) && IsValid(PC))
	{
		PC->Possess(Character);
	}
}

ACharacter* UPartyManagerComponent::GetAliveCharacter() const
{
	for (const TPair<FPrimaryAssetId, TObjectPtr<ACharacterBase>>& Kv : PartyCharacters)
	{
		ACharacterBase* Actor = Kv.Value.Get();
		if (IsValid(Actor) && Actor->IsAlive())
		{
			return Actor;
		}
	}
	return nullptr;
}


APlayerController* UPartyManagerComponent::GetControllerWithAuthority() const
{
	APlayerController* PC = Cast<APlayerController>(GetOwner());
	const ENetRole OwnerRole = GetOwnerRole();
	if (OwnerRole != ENetRole::ROLE_Authority || !IsValid(PC))
	{
		return nullptr;
	}
	return PC;
}






void UPartyManagerComponent::UpdatePartyOrder()
{
	if (SourceType == EAssetQuerySource::Instance)
	{
		PartyStorage->GetAllCharacters(PartyOrder);
	}
}

void UPartyManagerComponent::UpdateSpawnData()
{
	if (SourceType == EAssetQuerySource::Instance)
	{
		SpawnData.Empty();
		for (const FPrimaryAssetId& AssetId : PartyOrder)
		{
			FCharacterData Data;
			Data.AssetId = AssetId;
			Data.SourceType = EAssetQuerySource::Instance;

			SpawnData.Add(Data);
		}
	}
}

void UPartyManagerComponent::RemovePreviousParty()
{
	TArray<FPrimaryAssetId> CurrentIds;
	PartyCharacters.GetKeys(CurrentIds);
	
	for (const FPrimaryAssetId& AssetId : CurrentIds)
	{
		if (!PartyOrder.Contains(AssetId))
		{
			UnregisterCharacter(AssetId);
		}
	}
}


void UPartyManagerComponent::SpawnParty()
{
	UpdatePartyOrder();
	UpdateSpawnData();
	RemovePreviousParty();

	if (PartyOrder.Num() == 0)
	{
		return;
	}

	AssetManager->CancelFetch(_SpawnId);

	_SpawnId = FGuid::NewGuid();
	_SpawnLocation = PartyStorage->GetPartyLocation(*GetWorld()->GetMapName());

	const TArray<FName>& AssetBundles = UPartySettings::Get()->CharacterBundles;
	//AssetBundles.Add(TEXT("Character"));
	//AssetBundles.Add(TEXT("Ability"));

	TWeakObjectPtr<UPartyManagerComponent> WeakThis(this);
	TFuture<FLatentLoadedAssets<UCharacterAsset>> Future = AssetManager->FetchPrimaryAssets<UCharacterAsset>(_SpawnId, PartyOrder, AssetBundles, false);
	Future.Next([WeakThis](const FLatentLoadedAssets<UCharacterAsset>& Result)
		{
			UPartyManagerComponent* This = WeakThis.Get();
			if (IsValid(This) && Result.IsCompleted())
			{
				This->SpawnParty_Internal();
			}
		}
	);
}

void UPartyManagerComponent::SpawnParty_Internal()
{
	for (const FCharacterData& Data : SpawnData)
	{
		SpawnCharacter(Data.AssetId, Data);
	}

	PossessAliveCharacter();
}



void UPartyManagerComponent::SpawnCharacter(const FPrimaryAssetId& AssetId, const FCharacterData& Data)
{
	const UCharacterAsset* Asset = AssetManager->GetPrimaryAssetObject<UCharacterAsset>(AssetId);
	if (!IsValid(Asset) || PartyCharacters.Contains(AssetId))
	{
		LOG_ERROR(LogCharacterParty, TEXT("Character asset is invalid or already spawned"));
		return;
	}

	UClass* CharacterClass = Asset->CharacterClass.Get();
	if (!CharacterClass)
	{
		LOG_ERROR(LogCharacterParty, TEXT("CharacterClass is invalid"));
		return;
	}

	FTransform SpawnTransform;
	ACharacterBase* Character = GetWorld()->SpawnActorDeferred<ACharacterBase>(CharacterClass, SpawnTransform, GetOwner(), nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	if (!IsValid(Character))
	{
		LOG_ERROR(LogCharacterParty, TEXT("Spawned character is invalid"));
		return;
	}

	Character->CharacterAsset = Asset;
	Character->CharacterData = Data;
	Character->InitializeCharacter();
	Character->FinishSpawning(SpawnTransform);

	RegisterCharacter(AssetId, Character);
}


void UPartyManagerComponent::RegisterCharacter(const FPrimaryAssetId& AssetId, ACharacterBase* Character)
{
	PartyCharacters.Add(AssetId, Character);
}

void UPartyManagerComponent::UnregisterCharacter(const FPrimaryAssetId& AssetId)
{
	ACharacterBase* Character = PartyCharacters.FindAndRemoveChecked(AssetId);
	if (IsValid(Character))
	{
		Character->DeinitializeCharacter();
		Character->Destroy();
	}
}
