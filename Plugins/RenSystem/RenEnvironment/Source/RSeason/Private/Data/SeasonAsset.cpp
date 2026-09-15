// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Data/SeasonAsset.h"

// Engine Headers
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

// Project Headers
#if WITH_EDITOR
#include "Core/SeasonSettings.h"
#include "Data/SeasonWorldConfig.h"
#include "MaterialLibrary.h"
#include "WorldFragmentSettings.h"
#endif


FPrimaryAssetId USeasonAsset::GetPrimaryAssetId() const
{
    return FPrimaryAssetId(USeasonAsset::GetPrimaryAssetType(), GetFName());
}

FPrimaryAssetType USeasonAsset::GetPrimaryAssetType()
{
    return FPrimaryAssetType(TEXT("Season"));
}

#if WITH_EDITOR
void USeasonAsset::EditorApplySeasonToWorld() const
{
    EditorApplySeasonToWorld(1.0f);
}

void USeasonAsset::EditorApplySeasonToWorld(float Alpha) const
{
    if (!GEditor)
    {
        return;
    }

    UWorld* World = GEditor->GetEditorWorldContext().World();
    if (IsValid(World))
    {
        AWorldFragmentSettings* WorldSettings = Cast<AWorldFragmentSettings>(World->GetWorldSettings());
        if (!IsValid(WorldSettings))
        {
            FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Failed to get world settings")));
            return;
        }

        const USeasonSettings* Settings = USeasonSettings::Get();
        const USeasonWorldConfig* WorldConfig = WorldSettings->FindConfigByClass<USeasonWorldConfig>();
        if (!IsValid(WorldConfig))
        {
            FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Unable to get season world config")));
            return;
        }

        UMaterialParameterCollectionInstance* MPCInstance = World->GetParameterCollectionInstance(WorldConfig->SeasonMPC);
        if (!MPCInstance)
        {
            FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Unable to get season MPC")));
            return;
        }

        FMaterialSurfaceProperty ResultSurface;
        FMaterialSurfaceProperty DefaultSurface;

        FMaterialLibrary::LerpSurfaceProperty(DefaultSurface, SurfaceProperty, Alpha, ResultSurface);
        FMaterialLibrary::SetSurfaceProperty(ResultSurface, MPCInstance, Settings->SurfaceTint, Settings->SurfaceSROW, Settings->SurfaceDCMA);
    }
}

EDataValidationResult USeasonAsset::IsDataValid(FDataValidationContext& Context) const
{
    EDataValidationResult Result = Super::IsDataValid(Context);

    if (!IsValid(TransitionCurve))
    {
        Context.AddError(FText::FromString("Invalid transition curve"));
        return EDataValidationResult::Invalid;
    }

    return Result;
}
#endif

