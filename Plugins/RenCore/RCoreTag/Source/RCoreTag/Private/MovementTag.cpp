// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "MovementTag.h"



// Gameplay Tags
UE_DEFINE_GAMEPLAY_TAG(TAG_Movement_Mode_Walk, "State.Movement.Mode.Walk");
UE_DEFINE_GAMEPLAY_TAG(TAG_Movement_Mode_Fall, "State.Movement.Mode.Fall");
UE_DEFINE_GAMEPLAY_TAG(TAG_Movement_Mode_Fly, "State.Movement.Mode.Fly");
UE_DEFINE_GAMEPLAY_TAG(TAG_Movement_Mode_Swim, "State.Movement.Mode.Swim");

UE_DEFINE_GAMEPLAY_TAG(TAG_Movement_Jump, "Event.Movement.Jump");
UE_DEFINE_GAMEPLAY_TAG(TAG_Movement_Sprint, "Event.Movement.Sprint");

UE_DEFINE_GAMEPLAY_TAG(TAG_Movement_Ground_Idle, "State.Movement.Ground.Idle");
UE_DEFINE_GAMEPLAY_TAG(TAG_Movement_Ground_Sprint, "State.Movement.Ground.Sprint");


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