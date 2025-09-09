// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Library/AbilitySystemLibrary.h"

// Engine Headers
#include "AbilitySystemComponent.h"

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
