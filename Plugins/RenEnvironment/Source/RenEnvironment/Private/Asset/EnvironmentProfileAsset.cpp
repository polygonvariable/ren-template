// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Asset/EnvironmentProfileAsset.h"



UEnvironmentFogProfileAsset::UEnvironmentFogProfileAsset()
{
	ProfileType = EEnvironmentProfileType::Fog;
}

UEnvironmentLightProfileAsset::UEnvironmentLightProfileAsset()
{
	ProfileType = EEnvironmentProfileType::Light;
}

UEnvironmentAtmosphereProfileAsset::UEnvironmentAtmosphereProfileAsset()
{
	ProfileType = EEnvironmentProfileType::Atmosphere;
}

