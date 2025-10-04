// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "StorageUtils.h"

// Engine Headers

// Project Headers
#include "RCoreLibrary/Public/SubsystemUtils.h"
#include "RCoreStorage/Public/StorageProviderInterface.h"



UObject* StorageUtils::GetStorageInterface(UGameInstance* Context)
{
	IStorageProviderInterface* StorageProvider = SubsystemUtils::GetSubsystemInterface<UGameInstance, UGameInstanceSubsystem, IStorageProviderInterface>(Context);
	if (!StorageProvider)
	{
		return nullptr;
	}
	
	return StorageProvider->GetLocalStorage();
}

