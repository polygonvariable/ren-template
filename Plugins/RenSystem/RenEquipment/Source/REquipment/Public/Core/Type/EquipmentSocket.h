// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Generated Headers
#include "EquipmentSocket.generated.h"


/**
 *
 */
USTRUCT()
struct FEquipmentSocket
{

	GENERATED_BODY()

public:

	FEquipmentSocket() {};
	FEquipmentSocket(int InSlotId, bool bInUseComponent, FName InSocketName)
	{
		SlotId = InSlotId;
		bUseComponent = bInUseComponent;
		SocketName = InSocketName;
	};

	UPROPERTY(EditAnywhere)
	int SlotId = 10;

	UPROPERTY(EditAnywhere)
	bool bUseComponent = true;

	UPROPERTY(EditAnywhere)
	FName SocketName = NAME_None;

	UPROPERTY()
	FTransform SocketTransform = FTransform::Identity;

	bool IsValid()
	{
		return SocketName.IsValid();
	}

};

