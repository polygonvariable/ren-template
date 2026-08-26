// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Generated Headers
#include "EquipmentSocketInfo.generated.h"


/**
 * 
 */
USTRUCT()
struct FEquipmentSocketInfo
{

	GENERATED_BODY()

public:

	FEquipmentSocketInfo() {};
	FEquipmentSocketInfo(bool bInUseComponent, FName InSocketName) : bUseComponent(bInUseComponent), SocketName(InSocketName) {};


	UPROPERTY(EditAnywhere)
	bool bUseComponent = true;

	UPROPERTY(EditAnywhere)
	FName SocketName = NAME_None;

	UPROPERTY()
	FTransform SocketTransform = FTransform::Identity;


	bool IsValid() const
	{
		return SocketName.IsValid();
	}

};

