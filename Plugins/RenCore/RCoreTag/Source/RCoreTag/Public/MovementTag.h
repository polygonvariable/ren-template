// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "NativeGameplayTags.h"



// ~ Temporary
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Temporary_Damage);
// ~ End of Temporary


// ~ Abilities That Use Stamina
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Stamina_Ability);
// ~ End of Abilities That Use Stamina


// ~ Stamina State
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Stamina_Draining);
// ~ End of Stamina State


// ~ Movement State
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Movement_Mode_NavWalking);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Movement_Mode_Walking);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Movement_Mode_Falling);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Movement_Mode_Flying);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Movement_Mode_Swimming);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Movement_Mode_None);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Movement_Mode_Custom);
// ~ End of Movement State


// ~ Ground Movement State
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Movement_Ground_Idling);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Movement_Ground_Sprinting);
// ~ End of Ground Movement State


// ~ Air Movement State
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Movement_Air_Gliding);
// ~ End of Air Movement State


// ~ Movement Abilities
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Movement_Jump);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Movement_Sprint);
// ~ End of Movement Abilities

