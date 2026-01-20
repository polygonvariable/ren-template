// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "MovementTag.h"



// Gameplay Tags

UE_DEFINE_GAMEPLAY_TAG(TAG_Movement_Jump, "Event.Movement.Jump");
UE_DEFINE_GAMEPLAY_TAG(TAG_Movement_Sprint, "Event.Movement.Sprint");


UE_DEFINE_GAMEPLAY_TAG(TAG_Temporary_Damage, "Temporary.Damage");



// Deprecated
// UE_DEFINE_GAMEPLAY_TAG(TAG_Stamina_Consume, "Stamina.Consume");
// UE_DEFINE_GAMEPLAY_TAG(TAG_Stamina_Recover, "Stamina.Recover");
// ~ Deprecated


UE_DEFINE_GAMEPLAY_TAG(TAG_Stamina_Ability, "Ability.Uses.Stamina");

UE_DEFINE_GAMEPLAY_TAG(TAG_Stamina_Draining, "State.Stamina.Draining");
UE_DEFINE_GAMEPLAY_TAG(TAG_Stamina_Regenerating, "State.Stamina.Regenerating");

UE_DEFINE_GAMEPLAY_TAG(TAG_Stamina_Drain_Start, "Event.Stamina.Drain.Start");
UE_DEFINE_GAMEPLAY_TAG(TAG_Stamina_Drain_Started, "Event.Stamina.Drain.Started");
UE_DEFINE_GAMEPLAY_TAG(TAG_Stamina_Drain_Stop, "Event.Stamina.Drain.Stop");
UE_DEFINE_GAMEPLAY_TAG(TAG_Stamina_Drain_Stopped, "Event.Stamina.Drain.Stopped");

UE_DEFINE_GAMEPLAY_TAG(TAG_Stamina_Regenerate_Start, "Event.Stamina.Regenerate.Start");
UE_DEFINE_GAMEPLAY_TAG(TAG_Stamina_Regenerate_Stop, "Event.Stamina.Regenerate.Stop");


UE_DEFINE_GAMEPLAY_TAG(TAG_Movement_Mode_NavWalking, "State.Movement.Mode.NavWalking");
UE_DEFINE_GAMEPLAY_TAG(TAG_Movement_Mode_Walking, "State.Movement.Mode.Walking");
UE_DEFINE_GAMEPLAY_TAG(TAG_Movement_Mode_Falling, "State.Movement.Mode.Falling");
UE_DEFINE_GAMEPLAY_TAG(TAG_Movement_Mode_Flying, "State.Movement.Mode.Flying");
UE_DEFINE_GAMEPLAY_TAG(TAG_Movement_Mode_Swimming, "State.Movement.Mode.Swimming");
UE_DEFINE_GAMEPLAY_TAG(TAG_Movement_Mode_None, "State.Movement.Mode.None");
UE_DEFINE_GAMEPLAY_TAG(TAG_Movement_Mode_Custom, "State.Movement.Mode.Custom");

UE_DEFINE_GAMEPLAY_TAG(TAG_Movement_Ground_Idling, "State.Movement.Ground.Idling");
UE_DEFINE_GAMEPLAY_TAG(TAG_Movement_Ground_Sprinting, "State.Movement.Ground.Sprinting");

UE_DEFINE_GAMEPLAY_TAG(TAG_Movement_Air_Gliding, "State.Movement.Air.Gliding");

UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Movement_Jump, "Ability.Movement.Jump");
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Movement_Sprint, "Ability.Movement.Sprint");