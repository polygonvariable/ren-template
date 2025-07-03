// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"


#define BREAK_GET_WORLDSUBSYSTEM(SubsystemClass, SubsystemReference) \
	{ \
        UWorld* ___World___ = GetWorld(); \
        if (!IsValid(___World___)) { \
            UE_LOG(LogTemp, Error, TEXT("[%s] Invalid world"), *FString(__FUNCTION__)); \
            return; \
        } \
        SubsystemReference = ___World___->GetSubsystem<SubsystemClass>(); \
        if (!IsValid(SubsystemReference)) { \
            UE_LOG(LogTemp, Error, TEXT("[%s] Invalid world subsystem"), *FString(__FUNCTION__)); \
            return; \
        } \
    }

#define GET_GAMEINSTANCESUBSYSTEM_FROM_WORLD(SubsystemClass, SubsystemReference) \
    UWorld* World = GetWorld(); \
    if (!IsValid(World)) { \
        UE_LOG(LogTemp, Error, TEXT("[%s] Invalid world"), *FString(__FUNCTION__)); \
        return; \
    } \
    if (!IsValid(SubsystemReference)) \
    { \
        UGameInstance* GameInstance = World->GetGameInstance(); \
        GET_GAMEINSTANCESUBSYSTEM(GameInstance, SubsystemClass, SubsystemReference) \
    }


#define GET_GAMEINSTANCESUBSYSTEM_FROM_GAMEINSTANCE(SubsystemClass, SubsystemReference) \
    if (!IsValid(SubsystemReference)) \
    { \
        UGameInstance* GameInstance = GetGameInstance(); \
        GET_GAMEINSTANCESUBSYSTEM(GameInstance, SubsystemClass, SubsystemReference) \
    }


#define GET_GAMEINSTANCESUBSYSTEM(GameInstance, SubsystemClass, SubsystemReference) \
    if (!IsValid(GameInstance)) { \
        UE_LOG(LogTemp, Error, TEXT("[%s] Invalid game instance"), *FString(__FUNCTION__)); \
        return; \
    } \
    SubsystemReference = GameInstance->GetSubsystem<SubsystemClass>(); \
    if (!IsValid(SubsystemReference)) { \
        UE_LOG(LogTemp, Error, TEXT("[%s] Invalid game instance subsystem"), *FString(__FUNCTION__)); \
        return; \
    }

