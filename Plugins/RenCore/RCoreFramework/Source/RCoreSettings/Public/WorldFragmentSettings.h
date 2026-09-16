// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameFramework/WorldSettings.h"

// Generated Headers
#include "WorldFragmentSettings.generated.h"

// Forward Declarations
class UWorldConfigAsset;


/**
 * 
 */
UCLASS(MinimalAPI)
class AWorldFragmentSettings : public AWorldSettings
{

	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, Category = "World Configs")
	TArray<TObjectPtr<UWorldConfigAsset>> Configs;


	RCORESETTINGS_API const UWorldConfigAsset* FindConfigByClass(TSubclassOf<UWorldConfigAsset> InClass) const;

	template<typename T>
	const T* FindConfigByClass() const
	{
		return Cast<T>(FindConfigByClass(T::StaticClass()));
	}

public:

	template<typename T>
	static const T* GetConfigByClass(UWorld* World)
	{
		AWorldFragmentSettings* Settings = Cast<AWorldFragmentSettings>(World->GetWorldSettings());
		if (!IsValid(Settings))
		{
			return nullptr;
		}
		return Settings->FindConfigByClass<T>();
	}
	
};

