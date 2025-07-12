// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

// Project Headers
#include "RenWorldHistory/Public/WorldHistoryType.h"

// Generated Headers
#include "WorldHistorySubsystem.generated.h"

// Forward Declarations
class IWorldHistoryProviderInterface;



/**
 *
 */
UCLASS()
class UWorldHistorySubsystem : public UWorldSubsystem
{

	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	bool AddHistory(const FName Id, const bool bForceAdd = false);

	UFUNCTION(BlueprintCallable)
	bool RemoveHistory(const FName Id);

	UFUNCTION(BlueprintCallable)
	bool ContainsHistory(const FName Id) const;

	UFUNCTION(BlueprintCallable)
	bool GetHistoryDate(const FName Id, FDateTime& Date) const;

	UFUNCTION(BlueprintCallable)
	TEnumAsByte<EWorldHistoryStatus> GetHistoryStatus(const FName Id) const;

	UFUNCTION(BlueprintCallable)
	TEnumAsByte<EWorldHistoryCooldownStatus> GetHistoryCooldownStatus(const FName Id, const bool bIsOnlyOnce, const FTimespan CooldownTime) const;

protected:

	TWeakInterfacePtr<IWorldHistoryProviderInterface> WorldHistoryInterface;

	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;
	virtual void OnWorldComponentsUpdated(UWorld& InWorld) override;
	virtual void Deinitialize() override;

};

