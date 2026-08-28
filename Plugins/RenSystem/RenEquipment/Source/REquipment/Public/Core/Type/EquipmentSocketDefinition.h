// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Generated Headers
#include "EquipmentSocketDefinition.generated.h"


/**
 * 
 */
USTRUCT(BlueprintType)
struct FEquipmentSocketDefinition
{

	GENERATED_BODY()

public:

	FEquipmentSocketDefinition() {};
	FEquipmentSocketDefinition(bool bInUseComponent, FName InSocketName) : bUseComponent(bInUseComponent), SocketName(InSocketName) {};


	UPROPERTY(EditAnywhere)
	bool bUseComponent = true;

	UPROPERTY(EditAnywhere)
	FName SocketName = NAME_None;


	bool IsValid() const
	{
		return SocketName.IsValid();
	}

};

