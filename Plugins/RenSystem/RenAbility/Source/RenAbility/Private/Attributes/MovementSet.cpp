// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Attributes/MovementSet.h"

// Engine Headers
#include "GameplayEffectExtension.h"
#include "GameplayEffect.h"
#include "NativeGameplayTags.h"
#include "Kismet/GameplayStatics.h"
#include "InstancedStruct.h"
#include "Net/UnrealNetwork.h"

// Project Headers
#include "RCoreLibrary/Public/LogMacro.h"



void UMovementSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMaxGroundSpeedAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.0f);
	}
	else if (Attribute == GetMaxFlySpeedAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.0f);
	}
	else if (Attribute == GetMaxSwimSpeedAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.0f);
	}
}

void UMovementSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	const FGameplayAttribute& Attribute = Data.EvaluatedData.Attribute;

	if (Attribute == GetGroundSpeedAttribute())
	{
		float NewValue = FMath::Clamp(GetGroundSpeed(), 0.0f, GetMaxGroundSpeed());
		SetGroundSpeed(NewValue);
	}
	else if (Attribute == GetMaxGroundSpeedAttribute())
	{
		float NewValue = FMath::Max(GetMaxGroundSpeed(), 1.0f);
		SetMaxGroundSpeed(NewValue);
	}

	else if (Attribute == GetFlySpeedAttribute())
	{
		float NewValue = FMath::Clamp(GetFlySpeed(), 0.0f, GetMaxFlySpeed());
		SetFlySpeed(NewValue);
	}
	else if (Attribute == GetMaxFlySpeedAttribute())
	{
		float NewValue = FMath::Max(GetMaxFlySpeed(), 1.0f);
		SetMaxFlySpeed(NewValue);
	}

	else if (Attribute == GetSwimSpeedAttribute())
	{
		float NewValue = FMath::Clamp(GetSwimSpeed(), 0.0f, GetMaxSwimSpeed());
		SetSwimSpeed(NewValue);
	}
	else if (Attribute == GetMaxSwimSpeedAttribute())
	{
		float NewValue = FMath::Max(GetMaxSwimSpeed(), 1.0f);
		SetMaxSwimSpeed(NewValue);
	}
}

void UMovementSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UMovementSet, GroundSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMovementSet, MaxGroundSpeed, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UMovementSet, FlySpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMovementSet, MaxFlySpeed, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UMovementSet, SwimSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMovementSet, MaxSwimSpeed, COND_None, REPNOTIFY_Always);
}

void UMovementSet::OnRep_GroundSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMovementSet, GroundSpeed, OldValue);
}

void UMovementSet::OnRep_MaxGroundSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMovementSet, MaxGroundSpeed, OldValue);
}

void UMovementSet::OnRep_FlySpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMovementSet, FlySpeed, OldValue);
}

void UMovementSet::OnRep_MaxFlySpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMovementSet, MaxFlySpeed, OldValue);
}

void UMovementSet::OnRep_SwimSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMovementSet, SwimSpeed, OldValue);
}

void UMovementSet::OnRep_MaxSwimSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMovementSet, MaxSwimSpeed, OldValue);
}

