// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Util/SubsystemUtil.h"

// Engine Headers

// Project Headers



USubsystem* SubsystemUtil::GetSubsystemByInterface(UWorld* Context, UClass* InterfaceClass)
{
	if (!IsValid(Context))
	{
		return nullptr;
	}
	const TArray<UWorldSubsystem*>& Subsystems = Context->GetSubsystemArrayCopy<UWorldSubsystem>();
	for (UWorldSubsystem* Subsystem : Subsystems)
	{
		if (IsValid(Subsystem) && Subsystem->GetClass()->ImplementsInterface(InterfaceClass))
		{
			return Subsystem;
		}
	}
	return nullptr;
}

USubsystem* SubsystemUtil::GetSubsystemByInterface(UGameInstance* Context, UClass* InterfaceClass)
{
	if (!IsValid(Context))
	{
		return nullptr;
	}
	const TArray<UGameInstanceSubsystem*>& Subsystems = Context->GetSubsystemArrayCopy<UGameInstanceSubsystem>();
	for (UGameInstanceSubsystem* Subsystem : Subsystems)
	{
		if (IsValid(Subsystem) && Subsystem->GetClass()->ImplementsInterface(InterfaceClass))
		{
			return Subsystem;
		}
	}
	return nullptr;
}

