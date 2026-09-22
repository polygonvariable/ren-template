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

void UCharacterSpawnerComponent::BeginPlay()
{
	Super::BeginPlay();

	CreateCharacters();
}

void UCharacterSpawnerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	RemoveCharacters();

	Super::EndPlay(EndPlayReason);
}


void UCharacterSpawnerComponent::CreateCharacters()
{
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

	FAssetManagerUtil::CancelHandle(SpawnHandle);

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
	for (const TPair<FPrimaryAssetId, TObjectPtr<ACharacterBase>>& Kv : SpawnedCharacters)
	{
		UnregisterCharacter(Kv.Key);
	}
	SpawnedCharacters.Empty();
}


void UCharacterSpawnerComponent::HandleOnCharactersLoaded(TArray<FCharacterInitializationData> CharacterData)
{
	FAssetManagerUtil::ReleaseHandle(SpawnHandle);

	OnSpawnStarted();

	for (FCharacterInitializationData& Data : CharacterData)
	{
		SpawnCharacter(Data);
	}

	OnSpawnFinished();
}

void UCharacterSpawnerComponent::SpawnCharacter(FCharacterInitializationData& Data)
{
#if WITH_EDITOR
	const uint64 StartTime = FPlatformTime::Cycles64();
#endif

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

#if WITH_EDITOR
	const uint64 EndTime = FPlatformTime::Cycles64();
	const double TimeElapsed = FPlatformTime::ToMilliseconds64(EndTime - StartTime);
	LOG_WARNING(LogCharacterSpawner, TEXT("Character creation took: %.4f ms"), TimeElapsed);
#endif
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

