// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Generated Headers
#include "StorageInstance.generated.h"


/**
 * 
 */
USTRUCT()
struct FStorageInstance
{

	GENERATED_BODY()

public:

	FStorageInstance() {};
	FStorageInstance(FString InStorageClass, FName InStorageId) : StorageClass(InStorageClass), StorageId(InStorageId) {};
	
	UPROPERTY()
	FString StorageClass;

	UPROPERTY()
	FName StorageId;

	UPROPERTY()
	TArray<uint8> Data;

};

