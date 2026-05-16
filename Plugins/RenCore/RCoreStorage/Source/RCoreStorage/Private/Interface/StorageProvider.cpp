// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Interface/StorageProvider.h"

// Project Headers
#include "Util/SubsystemUtil.h"


IStorageProvider* IStorageProvider::Get(UWorld* World)
{
	if (!IsValid(World))
	{
		return nullptr;
	}
	return SubsystemUtil::GetSubsystemInterface<IStorageProvider>(World->GetGameInstance());
}

IStorageProvider* IStorageProvider::Get(UGameInstance* GameInstance)
{
	return SubsystemUtil::GetSubsystemInterface<IStorageProvider>(GameInstance);
}

