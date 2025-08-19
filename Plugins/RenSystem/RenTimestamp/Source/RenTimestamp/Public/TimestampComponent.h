// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

// Project Headers
#include "RenTimestamp/Public/TimestampStatus.h"

// Generated Headers
#include "TimestampComponent.generated.h"

// Forward Declarations
class UTimestampSubsystem;



/**
 * 
 */
UCLASS(ClassGroup = (Custom), MinimalAPI)
class UTimestampComponent : public UActorComponent
{

	GENERATED_BODY()

public:

	UTimestampComponent();


	UPROPERTY(EditAnywhere)
	FName TimestampId = NAME_None;

	TEnumAsByte<ETimestampStatus> GetStatus() const;

 	RENTIMESTAMP_API virtual bool AddTimestamp(bool bForceAdd);

protected:

	TWeakObjectPtr<UTimestampSubsystem> TimestampSubsystem;

public:

	virtual void Activate(bool bReset = false) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};



/**
 *
 */
UCLASS(ClassGroup = (Custom), MinimalAPI)
class UTimestampCooldownComponent : public UTimestampComponent
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FTimespan CooldownTime = FTimespan::FromMinutes(1);

	UPROPERTY(EditAnywhere)
	bool bOnlyOnce = true;

	RENTIMESTAMP_API TEnumAsByte<ETimestampCooldownStatus> GetCooldownStatus() const;

};

