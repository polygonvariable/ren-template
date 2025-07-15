// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

// Project Headers

// Generated Headers
#include "PrimaryAssetMap.generated.h"



/**
 *
 */
UCLASS()
class RENASSET_API UPrimaryAssetMap : public UPrimaryDataAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TMap<FName, TObjectPtr<UPrimaryDataAsset>> AssetMapping;

	template <typename T>
	T* GetAssetByName(FName Name)
	{
		TObjectPtr<UPrimaryDataAsset> Found = AssetMapping.FindRef(Name);
		if (IsValid(Found))
		{
			return Cast<T>(Found);
		}
		return nullptr;
	}

};

