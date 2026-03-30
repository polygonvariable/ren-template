// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

// Module Macros
#define REN_API RCORETAG_API


// Gameplay Tags
// ~ Deprecated
REN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Attack01_End);
REN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Attack02_End);
REN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Attack03_End);
REN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Attack04_End);
REN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Attack05_End);

REN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Equipment_Weapon_State_Ready);
REN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Equipment_Weapon_State_Aim);

REN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Equipment_Weapon_Attack_Basic_01);
REN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Equipment_Weapon_Attack_Basic_02);
REN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Equipment_Weapon_Attack_Basic_03);
REN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Equipment_Weapon_Attack_Basic_04);
REN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Equipment_Weapon_Attack_Basic_05);


REN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Equipment_Weapon_Slot_01);
REN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Equipment_Weapon_Slot_02);
REN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Equipment_Weapon_Slot_03);
REN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Equipment_Weapon_Slot_04);
REN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Equipment_Weapon_Slot_05);
// ~ End of Deprecated


// ~ Equipment Ability
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Uses_Equipment_Slot_01);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Uses_Equipment_Slot_02);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Uses_Equipment_Slot_03);
// ~ End of Equipment Ability

// ~ Equipment Event
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_Equipment_Slot_01);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_Equipment_Slot_02);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_Equipment_Slot_03);
// ~ End of Equipment Event

// ~ Equipment State
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Equipment_Slot_01);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Equipment_Slot_02);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Equipment_Slot_03);
// ~ End of Equipment State


// ~ Camera
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Equipment_Aim);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Camera_Aim);
// ~ End of Camera



// Module Macros
#undef REN_API

