// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Library/AbilitySystemLibrary.h"

// Engine Headers
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"

// Project Headers
#include "RCoreLibrary/Public/LogMacro.h"



void UAbilitySystemLibrary::ApplyGameplayEffectToTarget(AActor* Target, AActor* Caster, AActor* Causer, TSubclassOf<UGameplayEffect> EffectClass, int Level, TFunctionRef<void(FGameplayEffectContextHandle&)> CustomizeEffectContext)
{
	if (!IsValid(Target) || !IsValid(Caster) || !IsValid(Causer) || !IsValid(EffectClass))
	{
		return;
	}

	UAbilitySystemComponent* TargetASC = Target->FindComponentByClass<UAbilitySystemComponent>();
	UAbilitySystemComponent* CasterASC = Caster->FindComponentByClass<UAbilitySystemComponent>();

	if(!IsValid(TargetASC) || !IsValid(CasterASC))
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = CasterASC->MakeEffectContext();
	EffectContext.AddSourceObject(Caster);
	// EffectContext.AddHitResult();
	// EffectContext.AddActors(ContextActors);
	// EffectContext.AddOrigin(GetActorLocation());
	EffectContext.AddInstigator(Caster, Causer);

	CustomizeEffectContext(EffectContext);

	if (!EffectContext.IsValid())
	{
		return;
	}

	FGameplayEffectSpecHandle SpecHandle = CasterASC->MakeOutgoingSpec(EffectClass, Level, EffectContext);
	if (!SpecHandle.IsValid())
	{
		return;
	}

	CasterASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
}

void UAbilitySystemLibrary::CancelAbilityWithTags(AActor* Target, UPARAM(ref) const FGameplayTagContainer& InTags)
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent(Target);
	if (!IsValid(ASC))
	{
		return;
	}

	ASC->CancelAbilities(&InTags);
}

void UAbilitySystemLibrary::CancelAbilityWithoutTags(AActor* Target, UPARAM(ref) const FGameplayTagContainer& InTags)
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent(Target);
	if (!IsValid(ASC))
	{
		return;
	}
	
	ASC->CancelAbilities(nullptr, &InTags);
}






int UAbilitySystemLibrary::GetGameplayEffectCount(AActor* Target)
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent(Target);
	if (!IsValid(ASC))
	{
		return -1;
	}

	return ASC->GetNumActiveGameplayEffects();
}

UAbilitySystemComponent* UAbilitySystemLibrary::GetAbilitySystemComponent(AActor* Target)
{
	IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(Target);
	if (!AbilitySystemInterface)
	{
		return nullptr;
	}

	return AbilitySystemInterface->GetAbilitySystemComponent();
}





bool UAbilitySystemLibrary::IsGameplayEffectValid(AActor* Target, FActiveGameplayEffectHandle& Handle)
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent(Target);
	if (!IsValid(ASC))
	{
		return false;
	}

	const FActiveGameplayEffect* Effect = ASC->GetActiveGameplayEffect(Handle);
	return (Effect != nullptr);
}

void UAbilitySystemLibrary::InhibitGameplayEffect(AActor* InTarget, FActiveGameplayEffectHandle& InHandle, bool bInInhibit, bool bInInvokeCue, FActiveGameplayEffectHandle& OutHandle)
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent(InTarget);
	if (!IsValid(ASC))
	{
		OutHandle = FActiveGameplayEffectHandle();
		return;
	}

	OutHandle = ASC->SetActiveGameplayEffectInhibit(MoveTemp(InHandle), bInInhibit, bInInvokeCue);
}

