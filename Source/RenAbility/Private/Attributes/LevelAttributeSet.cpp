// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Attributes/LevelAttributeSet.h"

// Engine Headers
#include "GameplayEffectExtension.h"
#include "GameplayEffect.h"
#include "NativeGameplayTags.h"
#include "Kismet/GameplayStatics.h"
#include "InstancedStruct.h"

// Project Headers
#include "RenCore/Public/Macro/LogMacro.h"
#include "RenCore/Public/Tag/GameTags.h"




int ULevelAttributeSet::GetRankLevelCap() const
{
	return GetRank() * GetRankInterval();
}

bool ULevelAttributeSet::CanExperienceUp() const
{
	// int LevelCap = CurrentLevel * RankInterval;
	// return CurrentLevel < LevelCap || CurrentExperience < ExperienceMax;

	// return ULevelCalculationLibrary::CanLevelUp(GetLevel(), GetRankInterval(), GetExperience(), GetExperienceMax());
	return GetLevel() < GetRankLevelCap() || GetExperience() < GetExperienceMax();
}

bool ULevelAttributeSet::CanRankUp() const
{
	// return ULevelCalculationLibrary::CanRankUp(GetLevel(), GetRankInterval(), GetExperience(), GetExperienceMax());
	return GetLevel() >= GetRankLevelCap() && GetExperience() >= GetExperienceMax();
}

bool ULevelAttributeSet::TryAddExperience(float Value)
{
	//if (!CanExperienceUp())
	//{
	//	PRINT_ERROR(LogTemp, 5.0f, TEXT("Cannot level up"));
	//	return false;
	//}

	bool bLeveledUp = false;

	float TmpExperienceMax = GetExperienceMax();
	float TmpLevelMax = GetLevelMax();

	// SetExperience(FMath::Clamp(GetExperience() + Value, 0.0f, TmpExperienceMax));

	while (GetExperience() >= TmpExperienceMax && GetLevel() < GetRankLevelCap())
	{
		int NewExperience = FMath::Clamp(GetExperience() - TmpExperienceMax, 0.0f, TmpExperienceMax);
		int NewLevel = FMath::Clamp(GetLevel() + 1.0f, 1.0f, TmpLevelMax);

		SetExperience(NewExperience);
		SetLevel(NewLevel);

		PRINT_INFO(LogTemp, 1.0f, TEXT("Level Up, Current Level: %f"), GetLevel());
		bLeveledUp = true;
	}

	if (GetLevel() >= GetRankLevelCap() && GetExperience() > TmpExperienceMax)
	{
		SetExperience(TmpExperienceMax);
	}

	return bLeveledUp || Value > 0.0f;
}

bool ULevelAttributeSet::TryAddRank()
{
	//if (!CanRankUp())
	//{
	//	PRINT_ERROR(LogTemp, 5.0f, TEXT("Cannot rank up"));
	//	return false;
	//}

	int NewLevel = FMath::Clamp(GetLevel() + 1.0f, 1.0f, GetLevelMax());
	// int NewRank = FMath::Clamp(GetRank() + 1.0f, 1.0f, GetRankMax());

	SetExperience(0.0f);
	SetLevel(NewLevel);
	// SetRank(NewRank);

	PRINT_INFO(LogTemp, 1.0f, TEXT("Ranked Up"));
	return true;
}

bool ULevelAttributeSet::TrySetLevel(float Value)
{
	if (Value < 1.0f || Value > GetLevelMax())
	{
		PRINT_ERROR(LogTemp, 5.0f, TEXT("Invalid level value"));
		return false;
	}

	int NewLevel = FMath::Clamp(Value, 1.0f, GetLevelMax());
	int NewRank = FMath::Clamp((NewLevel - 1.0f) / GetRankInterval() + 1.0f, 1.0f, GetRankMax());

	SetExperience(0.0f);
	SetLevel(NewLevel);
	SetRank(NewRank);

	PRINT_INFO(LogTemp, 1.0f, TEXT("Level overridden to %d, Rank: %d"), NewLevel, NewRank);
	return true;
}



bool ULevelAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if (!Super::PreGameplayEffectExecute(Data))
	{
		PRINT_ERROR(LogTemp, 5.0f, TEXT("Level PreGameplayEffectExecute failed"));
		return false;
	}

	const FGameplayAttribute& Attribute = Data.EvaluatedData.Attribute;


	if (Attribute == GetExperienceAttribute())
	{
		PRINT_WARNING(LogTemp, 1.0f, TEXT("Experience is being modified"));
		return CanExperienceUp();
	}
	else if (Attribute == GetRankAttribute())
	{
		PRINT_WARNING(LogTemp, 1.0f, TEXT("Rank is being modified"));
		return CanRankUp();
	}

	return true;
}

void ULevelAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetLevelAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 1.0f, GetLevelMax());
	}
	else if (Attribute == GetLevelMaxAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 1.0f, FLT_MAX);
	}
	else if (Attribute == GetExperienceAttribute())
	{
		// The value is clamped in TryAddExperience
		// if its clamped here, then extra experience will be lost
		NewValue = FMath::Clamp(NewValue, 0.0f, FLT_MAX);
	}
	else if (Attribute == GetExperienceMaxAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, FLT_MAX);
	}
	else if (Attribute == GetRankAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 1.0f, GetRankMax());
	}
	else if (Attribute == GetRankMaxAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 1.0f, FLT_MAX);
	}
}

void ULevelAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	const FGameplayAttribute& Attribute = Data.EvaluatedData.Attribute;
	if (Attribute == GetExperienceAttribute())
	{
		TryAddExperience(Data.EvaluatedData.Magnitude);
	}
	else if (Attribute == GetRankAttribute())
	{
		TryAddRank();
	}

}



bool ULevelAttributeLibrary::CanExperienceUp(AActor* Target)
{
	const ULevelAttributeSet* LevelAttributeSet = GetLevelAttributeSet(Target);
	return LevelAttributeSet && LevelAttributeSet->CanExperienceUp();
}

bool ULevelAttributeLibrary::CanRankUp(AActor* Target)
{
	const ULevelAttributeSet* LevelAttributeSet = GetLevelAttributeSet(Target);
	return LevelAttributeSet && LevelAttributeSet->CanRankUp();
}

const ULevelAttributeSet* ULevelAttributeLibrary::GetLevelAttributeSet(AActor* Target)
{
	if (!IsValid(Target)) return nullptr;

	UAbilitySystemComponent* AbilityComponent = Target->FindComponentByClass<UAbilitySystemComponent>();
	if (!IsValid(AbilityComponent)) return nullptr;

	const UAttributeSet* AttributeSet = AbilityComponent->GetAttributeSet(ULevelAttributeSet::StaticClass());
	if (!IsValid(AttributeSet)) return nullptr;

	//AActor* Owner = AbilityComponent->GetOwner();
	//UAttributeSet* GrantedSet = NewObject<UAttributeSet>(Owner, ULevelAttributeSet::StaticClass());
	//AbilityComponent->AddAttributeSetSubobject(GrantedSet);
	//AbilityComponent->RemoveSpawnedAttribute(GrantedSet);
	//const ULevelAttributeSet* LevelAttributeSet = AbilityComponent->GetSet<ULevelAttributeSet>();
	//TArray<UAttributeSet*> Sets = AbilityComponent->GetSpawnedAttributes();
	//UAttributeSet* FoundSet = nullptr;
	//for(UAttributeSet* Set : Sets)
	//{
	//	if (Set->IsA(ULevelAttributeSet::StaticClass()))
	//	{
	//		FoundSet = Set;
	//		break;
	//	}
	//}
	//if(FoundSet) AbilityComponent->RemoveSpawnedAttribute(FoundSet);

	return Cast<ULevelAttributeSet>(AttributeSet);
}

bool ULevelCalculationLibrary::CanExperienceUp(int CurrentLevel, int RankInterval, int CurrentExperience, int ExperienceMax)
{
	int LevelCap = CurrentLevel * RankInterval;
	return CurrentLevel < LevelCap || CurrentExperience < ExperienceMax;
}

bool ULevelCalculationLibrary::CanRankUp(int CurrentLevel, int RankInterval, int CurrentExperience, int ExperienceMax)
{
	int LevelCap = CurrentLevel * RankInterval;
	return CurrentLevel >= LevelCap && CurrentExperience >= ExperienceMax;
}

