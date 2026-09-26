// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "CharacterSpawnerComponent.h"

// Engine Headers
#include "Engine/AssetManager.h"

// Project Headers
#include "Actor/CharacterBase.h"
#include "Core/AssetManagerUtil.h"
#include "Core/CharacterSpawnerSettings.h"
#include "Data/CharacterAsset.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"


UCharacterSpawnerComponent::UCharacterSpawnerComponent()
{
	bWantsInitializeComponent = true;
	bTickInEditor = false;
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	bAutoActivate = false;
	SetIsReplicatedByDefault(true);
}


void UCharacterSpawnerComponent::InitializeComponent()
{
	AssetManager = UAssetManager::GetIfInitialized();

	Super::InitializeComponent();
}

void UCharacterSpawnerComponent::UninitializeComponent()
{
	FAssetManagerUtil::CancelHandle(SpawnHandle);
	AssetManager = nullptr;

	Super::UninitializeComponent();
}


void UCharacterSpawnerComponent::CreateCharacters()
{
	FAssetManagerUtil::CancelHandle(SpawnHandle);

	if (SpawnedCharacters.Num() > 0)
	{
		LOG_ERROR(LogCharacterSpawner, TEXT("Characters are already spawned"));
		return;
	}

	TArray<FCharacterInitializationData> CharacterData;
	GetCharacterData(CharacterData);

	TArray<FPrimaryAssetId> AssetIds;
	for (const FCharacterInitializationData& Data : CharacterData)
	{
		if (Data.AssetId.IsValid())
		{
			AssetIds.Add(Data.AssetId);
		}
	}

	if (AssetIds.Num() == 0)
	{
		return;
	}

#if WITH_EDITOR
	AsyncStartTime = FPlatformTime::Cycles64();
#endif

	const UCharacterSpawnerSettings* Settings = UCharacterSpawnerSettings::Get();
	const TArray<FName>& AssetBundles = Settings->CharacterBundles;

	SpawnHandle = AssetManager->LoadPrimaryAssets(AssetIds, AssetBundles, FStreamableDelegate::CreateWeakLambda(this,
		[this, Data = MoveTemp(CharacterData)]()
		{
			HandleOnCharactersLoaded(Data);
		}
	));
}

void UCharacterSpawnerComponent::RemoveCharacters()
{
	FAssetManagerUtil::CancelHandle(SpawnHandle);

	for (const TPair<FPrimaryAssetId, TObjectPtr<ACharacterBase>>& Kv : SpawnedCharacters)
	{
		UnregisterCharacter(Kv.Key);
	}
	SpawnedCharacters.Empty();
}


void UCharacterSpawnerComponent::ShowCharacters()
{
	for (const TPair<FPrimaryAssetId, TObjectPtr<ACharacterBase>>& Kv : SpawnedCharacters)
	{
		ACharacterBase* Character = Kv.Value.Get();
		if (IsValid(Character))
		{
			Character->SetActorHiddenInGame(false);
		}
	}
}

void UCharacterSpawnerComponent::HideCharacters()
{
	FAssetManagerUtil::CancelHandle(SpawnHandle);

	for (const TPair<FPrimaryAssetId, TObjectPtr<ACharacterBase>>& Kv : SpawnedCharacters)
	{
		ACharacterBase* Character = Kv.Value.Get();
		if (IsValid(Character))
		{
			Character->SetActorHiddenInGame(true);
		}
	}
}


void UCharacterSpawnerComponent::HandleOnCharactersLoaded(TArray<FCharacterInitializationData> CharacterData)
{
#if WITH_EDITOR
	const uint64 AsyncEndTime = FPlatformTime::Cycles64();
	const double AsyncTimeElapsed = FPlatformTime::ToMilliseconds64(AsyncEndTime - AsyncStartTime);
	PRINT_WARNING(LogCharacterSpawner, 10.0f, TEXT("Character load time: %.4f ms"), AsyncTimeElapsed);

	const uint64 SpawnStartTime = FPlatformTime::Cycles64();
#endif

	OnSpawnStarted();

	for (FCharacterInitializationData& Data : CharacterData)
	{
		SpawnCharacter(Data);
	}

	OnSpawnFinished();

#if WITH_EDITOR
	const uint64 SpawnEndTime = FPlatformTime::Cycles64();
	const double SpawnTimeElapsed = FPlatformTime::ToMilliseconds64(SpawnEndTime - SpawnStartTime);
	PRINT_WARNING(LogCharacterSpawner, 10.0f, TEXT("Character spawn time: %.4f ms"), SpawnTimeElapsed);
	PRINT_WARNING(LogCharacterSpawner, 10.0f, TEXT("Character creation time: %.4f ms"), SpawnTimeElapsed + AsyncTimeElapsed);
#endif
}

void UCharacterSpawnerComponent::SpawnCharacter(FCharacterInitializationData& Data)
{
	const UCharacterAsset* Asset = AssetManager->GetPrimaryAssetObject<UCharacterAsset>(Data.AssetId);
	if (!IsValid(Asset))
	{
		LOG_ERROR(LogCharacterSpawner, TEXT("Character asset is invalid or already spawned"));
		return;
	}

	UClass* CharacterClass = Asset->CharacterClass.Get();
	if (!IsValid(CharacterClass))
	{
		LOG_ERROR(LogCharacterSpawner, TEXT("CharacterClass is invalid"));
		return;
	}

	ACharacterBase* Character = GetWorld()->SpawnActorDeferred<ACharacterBase>(CharacterClass, FTransform(), GetOwner(), nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	if (!IsValid(Character))
	{
		LOG_ERROR(LogCharacterSpawner, TEXT("Spawned character is invalid"));
		return;
	}

	Character->CharacterAsset = Asset;
	Character->CharacterData = Data;

	PreRegisterCharacter(Data.AssetId, Character);
	RegisterCharacter(Data.AssetId, Character);
}


void UCharacterSpawnerComponent::RegisterCharacter(const FPrimaryAssetId& AssetId, ACharacterBase* Character)
{
	Character->InitializeCharacter();
	Character->FinishSpawning(FTransform());

	SpawnedCharacters.Add(AssetId, Character);
}

void UCharacterSpawnerComponent::UnregisterCharacter(const FPrimaryAssetId& AssetId)
{
	const TObjectPtr<ACharacterBase>* FoundCharacter = SpawnedCharacters.Find(AssetId);
	if (FoundCharacter)
	{
		ACharacterBase* Character = FoundCharacter->Get();
		if (IsValid(Character))
		{
			Character->DeinitializeCharacter();
			Character->Destroy();
		}
	}
}


ACharacter* UCharacterSpawnerComponent::GetAliveCharacter() const
{
	for (const TPair<FPrimaryAssetId, TObjectPtr<ACharacterBase>>& Kv : SpawnedCharacters)
	{
		ACharacterBase* Character = Kv.Value.Get();
		if (IsValid(Character) && Character->IsAlive())
		{
			return Character;
		}
	}
	return nullptr;
}

