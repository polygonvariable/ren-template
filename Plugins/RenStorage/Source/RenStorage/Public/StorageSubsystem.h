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

	UPROPERTY(BlueprintReadOnly, Category = "Runtime")
	TObjectPtr<UStorage> Storage;



	UFUNCTION(BlueprintCallable)
	void ReadStorage(FName SlotId = "Default");

	UFUNCTION(BlueprintCallable)
	void UpdateStorage(FName SlotId = "Default");

	UFUNCTION(BlueprintCallable, Meta = (BlueprintPure))
	bool DoesStorageExist(FName SlotId = "Default");

	UFUNCTION(BlueprintCallable)
	UStorage* GetLocalStorage();


	virtual USaveGame* IGetLocalStorage() override;

protected:

	virtual bool ShouldCreateSubsystem(UObject* Object) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

};
