// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


/**
 *
 */
struct FStorageDefinition
{

public:

    FStorageDefinition() {};
    FStorageDefinition(FName InStorageId, TSubclassOf<UObject> InStorageClass, TSubclassOf<UObject> InManagerClass) : StorageId(InStorageId), StorageClass(InStorageClass), ManagerClass(InManagerClass) {};

    FName StorageId;
    TSubclassOf<UObject> StorageClass;
    TSubclassOf<UObject> ManagerClass;
    FString RemoteUrl;

    bool IsValid() const
    {
        return StorageClass != nullptr && StorageId.IsValid();
    }

    bool IsUrlValid() const
    {
        return !RemoteUrl.IsEmpty();
    }

};

