// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Async/AnimPlayMontage.h"

// Engine Headers

// Project Headers
#include "Component/RAbilitySystemComponent.h"
#include "RenGlobal/Public/Macro/LogMacro.h"



void UAnimPlayMontage::OnMontageBlendedIn(UAnimMontage* InMontage)
{
	OnBlendedIn.Broadcast();
}

void UAnimPlayMontage::OnMontageBlendingOut(UAnimMontage* InMontage, bool bInterrupted)
{
	OnBlendedOut.Broadcast(bInterrupted);
	EndTask();
}

void UAnimPlayMontage::OnMontageEnded(UAnimMontage* InMontage, bool bInterrupted)
{
	OnCompleted.Broadcast(bInterrupted);
	EndTask();
}

UAnimPlayMontage* UAnimPlayMontage::AnimPlayMontage(URAbilitySystemComponent* InRASC, UAnimMontage* InMontage, float InPlayRate, float InStartTime, float InBlendOutTime, bool bInStopAllMontages)
{
	UAnimPlayMontage* NewTask = NewObject<UAnimPlayMontage>();

	NewTask->RASC = InRASC;
	NewTask->Montage = InMontage;
	NewTask->PlayRate = InPlayRate;
	NewTask->StartTime = InStartTime;
	NewTask->BlendOutTime = InBlendOutTime;
	NewTask->bStopAllMontages = bInStopAllMontages;

	return NewTask;
}

void UAnimPlayMontage::EndTask()
{
	if (RASC.IsValid())
	{
		if (UAnimInstance* AnimInstance = RASC->GetActorAnimInstance())
		{
			if (FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveInstanceForMontage(Montage))
			{
				MontageInstance->OnMontageBlendedInEnded.Unbind();
				MontageInstance->OnMontageBlendingOutStarted.Unbind();
				MontageInstance->OnMontageEnded.Unbind();
			}
		}
		RASC->AnimStopMontage(Montage, BlendOutTime);
	}
	RASC.Reset();

	SetReadyToDestroy();
	MarkAsGarbage();
}

void UAnimPlayMontage::Activate()
{
	if (RASC.IsValid())
	{
		UAnimInstance* AnimInstance = RASC->GetActorAnimInstance();

		if (IsValid(AnimInstance) && RASC->AnimPlayMontage(Montage, PlayRate) > 0.0f)
		{
			OnStarted.Broadcast();

			BlendedInDelegate.BindUObject(this, &UAnimPlayMontage::OnMontageBlendedIn);
			AnimInstance->Montage_SetBlendedInDelegate(BlendedInDelegate, Montage);

			BlendingOutDelegate.BindUObject(this, &UAnimPlayMontage::OnMontageBlendingOut);
			AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, Montage);

			MontageEndedDelegate.BindUObject(this, &UAnimPlayMontage::OnMontageEnded);
			AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, Montage);

			return;
		}
	}

	OnCancelled.Broadcast();
}
