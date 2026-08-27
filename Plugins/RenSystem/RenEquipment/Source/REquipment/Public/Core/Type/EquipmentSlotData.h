// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Core/Type/EquipmentSocketInfo.h"

// Generated Headers
#include "EquipmentSlotData.generated.h"


/**
 * 
 */
USTRUCT()
struct FEquipmentSlotData
{

	GENERATED_BODY()

public:

	FEquipmentSlotData() {};
	FEquipmentSlotData(int InSlotId, int InInputId) : SlotId(InSlotId), InputId(InInputId) {};
	FEquipmentSlotData(int InSlotId, int InInputId, FEquipmentSocketInfo InAttachSocket, FEquipmentSocketInfo InDetachSocket) : SlotId(InSlotId), InputId(InInputId), AttachSocket(InAttachSocket), DetachSocket(InDetachSocket) {};


	UPROPERTY(EditAnywhere)
	int SlotId = 10;

	UPROPERTY(EditAnywhere)
	int InputId = 10;

	UPROPERTY(EditAnywhere)
	FEquipmentSocketInfo AttachSocket;

	UPROPERTY(EditAnywhere)
	FEquipmentSocketInfo DetachSocket;


	bool IsValid() const
	{
		return SlotId > 0;
	}

};

