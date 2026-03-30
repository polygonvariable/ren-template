// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

// Module Macros
#define REN_API RCORETAG_API



// Gameplay Tags
REN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Effect_Damage_Physical);
REN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Effect_Damage_Magical);

REN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_UI_Effect_Visible);
REN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_UI_Effect_Stackable);
REN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_UI_Effect_Duration);



// Module Macros
#undef REN_API

