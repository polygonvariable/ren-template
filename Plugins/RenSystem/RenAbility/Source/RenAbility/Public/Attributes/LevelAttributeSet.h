// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"

// Project Headers
#include "AbilityMacro.h"

// Generated Headers
#include "LevelAttributeSet.generated.h"



/**
 *
 */
UCLASS()
class ULevelAttributeSet : public UAttributeSet
{

	GENERATED_BODY()

public:

	// Level per Rank
	// int RankInterval = 10;


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData Experience;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData ExperienceMax;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData Level;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData LevelMax;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData Rank;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData RankMax;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData RankInterval;



	int GetRankLevelCap() const;

	bool CanExperienceUp() const;

	bool CanRankUp() const;

protected:

	bool TryAddExperience(float Value);

	bool TryAddRank();

	bool TrySetLevel(float Value);

public:

	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

public:

	ATTRIBUTE_ACCESSORS(ULevelAttributeSet, Level)
	ATTRIBUTE_ACCESSORS(ULevelAttributeSet, LevelMax)
	ATTRIBUTE_ACCESSORS(ULevelAttributeSet, Rank)
	ATTRIBUTE_ACCESSORS(ULevelAttributeSet, RankMax)
	ATTRIBUTE_ACCESSORS(ULevelAttributeSet, RankInterval)
	ATTRIBUTE_ACCESSORS(ULevelAttributeSet, Experience)
	ATTRIBUTE_ACCESSORS(ULevelAttributeSet, ExperienceMax)

};


/**
 *
 */
UCLASS()
class ULevelAttributeLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Meta = (DefaultToSelf = "Target"))
	static bool CanExperienceUp(AActor* Target);

	UFUNCTION(BlueprintCallable, Meta = (DefaultToSelf = "Target"))
	static bool CanRankUp(AActor* Target);

protected:

	static const ULevelAttributeSet* GetLevelAttributeSet(AActor* Target);

};

/**
 *
 */
UCLASS()
class ULevelCalculationLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static bool CanExperienceUp(int CurrentLevel, int RankInterval, int CurrentExperience, int ExperienceMax);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static bool CanRankUp(int CurrentLevel, int RankInterval, int CurrentExperience, int ExperienceMax);

};


