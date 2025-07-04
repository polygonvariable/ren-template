// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenCore/Public/Subsystem/RenGameInstanceSubsystem.h"
#include "RenCore/Public/Storage/StorageInterface.h"

// Generated Headers
#include "StorageSubsystem.generated.h"

// Forward Declarations
class UStorage;


/**
 * 
 */
UCLASS()
class RENSTORAGE_API UStorageSubsystem : public UGameInstanceSubsystem, public IStorageSubsystemInterface
{

	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	bool ReadStorage(FName SlotId = "Default", int UserIndex = 0);

	UFUNCTION(BlueprintCallable)
	bool UpdateStorage(FName SlotId = "Default", int UserIndex = 0);

	UFUNCTION(BlueprintCallable, Meta = (BlueprintPure))
	bool DoesStorageExist(FName SlotId = "Default", int UserIndex = 0);


	UFUNCTION(BlueprintCallable)
	UStorage* GetLocalStorage();


	virtual USaveGame* IGetLocalStorage() override;

protected:

	UPROPERTY()
	TObjectPtr<UStorage> CurrentStorage;

	FName CurrentSlotId = NAME_None;
	int CurrentUserIndex = 0;

	bool CreateNewStorage(FName SlotId, int UserIndex);

protected:

	virtual bool ShouldCreateSubsystem(UObject* Object) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

};
