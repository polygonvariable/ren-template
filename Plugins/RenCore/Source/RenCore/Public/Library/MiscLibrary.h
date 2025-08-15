// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"


#include "MiscLibrary.generated.h"



template <typename TSubsystemType, typename TContextType>
bool GetSubsystemReference(TContextType* Context, TObjectPtr<TSubsystemType>& OutSubsystem)
{
	if (IsValid(OutSubsystem)) return true;

	OutSubsystem = nullptr;
	if (!IsValid(Context)) return false;

	OutSubsystem = Context->GetSubsystem<TSubsystemType>();
	return IsValid(OutSubsystem);
}

template <typename TSubsystem, typename TContext>
TSubsystem* GetSubsystem(TContext* Context)
{
	if (!IsValid(Context)) return nullptr;
	return Context->GetSubsystem<TSubsystem>();
}


template <typename TSubsystem>
TSubsystem* GetGameInstanceSubsystem(UWorld* WorldContext)
{
	if (!IsValid(WorldContext)) return nullptr;

	UGameInstance* GameInstance = WorldContext->GetGameInstance();
	if (!IsValid(GameInstance)) return nullptr;

	return GameInstance->GetSubsystem<TSubsystem>();
}



UCLASS()
class RENCORE_API UMiscLibrary : public UObject
{

	GENERATED_BODY()

public:

	static bool IsInGameWorld(UWorld* World)
	{
		return World &&
		(
			World->WorldType == EWorldType::Game ||
			World->WorldType == EWorldType::GamePreview ||
			World->WorldType == EWorldType::GameRPC ||
			World->WorldType == EWorldType::PIE
		);
	}

};






class SubsystemUtils
{

public:

	template <typename TContext, typename TSubsystem, typename TInterface>
	static TSubsystem* GetSubsystemByInterface(TContext* Context)
	{
		if (!IsValid(Context)) return nullptr;

		const TArray<TSubsystem*>& Subsystems = Context->GetSubsystemArray<TSubsystem>();

		for (TSubsystem* Subsystem : Subsystems)
		{
			if (IsValid(Subsystem) && Subsystem->GetClass()->ImplementsInterface(TInterface::UClassType::StaticClass()))
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

		if (TSubsystem* Subsystem = GetSubsystemByInterface<TContext, TSubsystem, TInterface>(Context))
		{
			if (TInterface* Interface = Cast<TInterface>(Subsystem))
			{
				return Interface;
			}
		}

		return nullptr;
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



class TimerUtils
{

public:

	template <typename UserClass>
	static bool StartTimer(FTimerHandle& TimerHandle, UserClass* Object, typename FTimerDelegate::TMethodPtr<UserClass> InTimerMethod, float Rate)
	{
		FTimerManager& TimerManager = Object->GetWorld()->GetTimerManager();
		if (TimerManager.TimerExists(TimerHandle))
		{
			if (!TimerManager.IsTimerPaused(TimerHandle))
			{
				return false;
			}
			TimerManager.UnPauseTimer(TimerHandle);
		}
		else
		{
			TimerManager.SetTimer(TimerHandle, Object, InTimerMethod, Rate, FTimerManagerTimerParameters{ .bLoop = true, .bMaxOncePerFrame = true });
		}

		return true;
	}

	template <typename UserClass>
	static bool PauseTimer(FTimerHandle& TimerHandle, UserClass* Object)
	{
		FTimerManager& TimerManager = Object->GetWorld()->GetTimerManager();
		if (TimerManager.TimerExists(TimerHandle))
		{
			TimerManager.PauseTimer(TimerHandle);
			return true;
		}

		return false;
	}

	template <typename UserClass>
	static void ClearTimer(FTimerHandle& TimerHandle, UserClass* Object)
	{
		FTimerManager& TimerManager = Object->GetWorld()->GetTimerManager();
		TimerManager.ClearAllTimersForObject(Object);
		TimerHandle.Invalidate();
	}

};

