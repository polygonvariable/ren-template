// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Data/CharacterAsset.h"

// Engine Headers
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif


FPrimaryAssetId UCharacterAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(UCharacterAsset::GetPrimaryAssetType(), GetFName());
}

FPrimaryAssetType UCharacterAsset::GetPrimaryAssetType()
{
	return TEXT("Character");
}

#if WITH_EDITOR
EDataValidationResult UCharacterAsset::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

    if (SkeletonMesh.IsNull())
    {
        Context.AddError(FText::FromString("Skeleton mesh is invalid"));
        return EDataValidationResult::Invalid;
    }

    if (AnimBlueprint.IsNull())
    {
        Context.AddError(FText::FromString("Animation blueprint is invalid"));
        return EDataValidationResult::Invalid;
    }

    if (CharacterClass.IsNull())
    {
        Context.AddError(FText::FromString("Character class is invalid"));
        return EDataValidationResult::Invalid;
    }

    if (CharacterTemplate.IsNull())
    {
        Context.AddError(FText::FromString("Character template is invalid"));
        return EDataValidationResult::Invalid;
    }

    return Result;
}
#endif

