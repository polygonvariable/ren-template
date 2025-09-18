// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Quest/QuestAsset.h"

// Engine Headers

// Project Headers



#if WITH_EDITOR

void UQuestAsset::PostPropertyUpdate()
{
}

#endif

FPrimaryAssetId UQuestAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(TEXT("Quest"), GetFName());
}

