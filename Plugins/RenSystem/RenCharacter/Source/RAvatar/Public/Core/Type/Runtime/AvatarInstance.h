// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Core/Type/AscensionData.h"

// Generated Headers
#include "AvatarInstance.generated.h"


/**
 * 
 */
USTRUCT(BlueprintType)
struct FAvatarInstance
{

	GENERATED_BODY()

public:

	FAvatarInstance() {};
	FAvatarInstance(const FGuid& InAvatarId, int InHealth) : AvatarId(InAvatarId), Health(InHealth) {};

	UPROPERTY(EditAnywhere, SaveGame)
	FGuid AvatarId;

	UPROPERTY(EditAnywhere, SaveGame)
	int Health = 0;

	UPROPERTY(EditAnywhere, SaveGame)
	FAscensionData Ascension;

	void Reset()
	{
		AvatarId.Invalidate();
		Health = 0;
		Ascension.Reset();
	}

	void Sanitize()
	{
		Health = FMath::Max(0, Health);
		Ascension.Sanitize();
	}

	FString ToString() const
	{
		FString Detail = TEXT("Avatar Id: ") + AvatarId.ToString();
		Detail += TEXT("\nHealth: ") + FString::FromInt(Health);
		Detail += TEXT("\nAscension: ") + Ascension.ToString();
		return Detail;
	}

	friend inline bool operator == (const FAvatarInstance& A, const FAvatarInstance& B)
	{
		return A.AvatarId == B.AvatarId && A.Health == B.Health && A.Ascension == B.Ascension;
	}

	friend inline uint32 GetTypeHash(const FAvatarInstance& A)
	{
		uint32 Hash = GetTypeHash(A.AvatarId);
		Hash = HashCombineFast(Hash, GetTypeHash(A.Health));
		Hash = HashCombineFast(Hash, GetTypeHash(A.Ascension));
		return Hash;
	}

};

