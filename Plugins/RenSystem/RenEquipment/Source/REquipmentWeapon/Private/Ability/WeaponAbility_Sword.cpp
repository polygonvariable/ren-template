// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Ability/WeaponAbility_Sword.h"

// Engine Headers
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Kismet/KismetSystemLibrary.h"


UWeaponAbility_Sword::UWeaponAbility_Sword()
{
	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_Destructible));

	SwapEventTag = FGameplayTag::RequestGameplayTag(TEXT("Event.Combat.Swap"));
	AttackEventTag = FGameplayTag::RequestGameplayTag(TEXT("Event.Combat.Attack"));

	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UWeaponAbility_Sword::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsValid(SwapTask))
	{
		SwapTask->EndTask();
		SwapTask = nullptr;
	}

	if (IsValid(AttackTask))
	{
		AttackTask->EndTask();
		AttackTask = nullptr;
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

#if WITH_EDITOR
EDataValidationResult UWeaponAbility_Sword::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	if (RapidAttacks.Num() == 0)
	{
		Context.AddError(FText::FromString("Attck montages is empty"));
		return EDataValidationResult::Invalid;
	}

	if (!IsValid(ChargedAttack))
	{
		Context.AddError(FText::FromString("Attck montage is invalid"));
		return EDataValidationResult::Invalid;
	}

	if (!SwapEventTag.IsValid() || !AttackEventTag.IsValid())
	{
		Context.AddError(FText::FromString("Event tags is invalid"));
		return EDataValidationResult::Invalid;
	}

	return Result;
}
#endif


void UWeaponAbility_Sword::OnInputSingle()
{
	if (!IsActive() || !RapidAttacks.IsValidIndex(MontageIndex))
	{
		return;
	}

	PlayAndRegisterMontage(RapidAttacks[MontageIndex]);

	MontageIndex++;
	if (MontageIndex >= RapidAttacks.Num())
	{
		MontageIndex = 0;
	}
}

void UWeaponAbility_Sword::OnInputHold()
{
	if (!IsActive())
	{
		return;
	}

	MontageIndex = 0;
	PlayAndRegisterMontage(ChargedAttack);
}

void UWeaponAbility_Sword::PlayAndRegisterMontage(UAnimMontage* Montage)
{
	AttackTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, AttackEventTag);
	AttackTask->EventReceived.AddDynamic(this, &UWeaponAbility_Sword::HandleOnAttackEventReceived);
	AttackTask->ReadyForActivation();

	SwapTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, SwapEventTag);
	SwapTask->EventReceived.AddDynamic(this, &UWeaponAbility_Sword::HandleOnSwapEventReceived);
	SwapTask->ReadyForActivation();

	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, Montage, 1.0f, NAME_None, false);
	MontageTask->OnCompleted.AddDynamic(this, &UWeaponAbility_Sword::HandleOnMontageEvent);
	MontageTask->OnInterrupted.AddDynamic(this, &UWeaponAbility_Sword::HandleOnMontageEvent);
	MontageTask->OnCancelled.AddDynamic(this, &UWeaponAbility_Sword::HandleOnMontageEvent);
	MontageTask->ReadyForActivation();
}


void UWeaponAbility_Sword::HandleOnMontageEvent()
{
	if (IsActive())
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}
}

void UWeaponAbility_Sword::HandleOnAttackEventReceived(FGameplayEventData Payload)
{
	AActor* Actor = GetAvatarActorFromActorInfo();
	FVector Location = Actor->GetActorLocation();

	TArray<FHitResult> Hits;
	bool bSuccess = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), Location, Location, TraceDistance, TraceObjects, false, TArray<AActor*>(), EDrawDebugTrace::ForDuration, Hits, true);
	if (!bSuccess)
	{
		return;
	}
}

void UWeaponAbility_Sword::HandleOnSwapEventReceived(FGameplayEventData Payload)
{
	if (IsActive())
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}
}

