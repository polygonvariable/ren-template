// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/AvatarEntry.h"

// Project Headers
#include "Definition/Runtime/AvatarInstance.h"


FGuid UAvatarEntry::GetAssetInstanceId() const
{
	if (AvatarInstance)
	{
		return AvatarInstance->AvatarId;
	}
	return FGuid();
}

void UAvatarEntry::ResetData()
{
	AvatarInstance = nullptr;
	Super::ResetData();
}

