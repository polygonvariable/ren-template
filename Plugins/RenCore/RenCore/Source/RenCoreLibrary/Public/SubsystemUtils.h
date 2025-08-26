// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"



class SubsystemUtils
{

public:

	template <typename TContext, typename TSubsystem, typename TInterface>
	static TSubsystem* GetSubsystemByInterface(TContext* Context)
	{
		if (!IsValid(Context)) return nullptr;

		const TArray<TSubsystem*>& Subsystems = Context->GetSubsystemArray<TSubsystem>();
		UClass* SubsystemClass = TInterface::UClassType::StaticClass();

		for (TSubsystem* Subsystem : Subsystems)
		{
			if (IsValid(Subsystem) && Subsystem->GetClass()->ImplementsInterface(SubsystemClass))
			{
				return Subsystem;
			}
		}

		return nullptr;
	}

	template <typename TContext, typename TSubsystem, typename TInterface>
	static TInterface* GetSubsystemInterface(TContext* Context)
	{
		if (!IsValid(Context)) return nullptr;

		return Cast<TInterface>(GetSubsystemByInterface<TContext, TSubsystem, TInterface>(Context));
	}

	template <typename TInterface>
	static TInterface* GetSubsystemInterface(UWorld* Context)
	{
		return GetSubsystemInterface<UWorld, UWorldSubsystem, TInterface>(Context);
	}

	template <typename TInterface>
	static TInterface* GetSubsystemInterface(UGameInstance* Context)
	{
		return GetSubsystemInterface<UGameInstance, UGameInstanceSubsystem, TInterface>(Context);
	}

};

