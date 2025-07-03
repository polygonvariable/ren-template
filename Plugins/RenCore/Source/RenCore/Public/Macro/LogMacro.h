// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Log Categories
//RENCORE_API DECLARE_LOG_CATEGORY_EXTERN(LogTemp, Log, All);
//
//RENCORE_API DECLARE_LOG_CATEGORY_EXTERN(LogTemp, Log, All);
//
//RENCORE_API DECLARE_LOG_CATEGORY_EXTERN(LogTemp, Log, All);
//RENCORE_API DECLARE_LOG_CATEGORY_EXTERN(LogTemp, Log, All);



#define REN_LOG(Category, Verbosity, Text, ...) \
    { \
        const FString __InternalName__ = this ? this->GetClass()->GetName() : TEXT("Static"); \
        const FString __InternalMessage__ = FString::Printf(TEXT("[%s::%s] " Text), *__InternalName__, *FString(__FUNCTION__), ##__VA_ARGS__); \
        UE_LOG(Category, Verbosity, TEXT("%s"), *__InternalMessage__) \
    }

#define REN_PRINT(Message, Time, Color) \
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, Time, Color, Message);

#define REN_PRINT_LOG(Category, Verbosity, Time, Color, Text, ...) \
    { \
        const FString __InternalName__ = this ? this->GetClass()->GetName() : TEXT("Static"); \
        const FString __InternalMessage__ = FString::Printf(TEXT("[%s::%s] " Text), *__InternalName__, *FString(__FUNCTION__), ##__VA_ARGS__); \
        const FString __PrintMessage__ = FString::Printf(Text, ##__VA_ARGS__); \
        UE_LOG(Category, Verbosity, TEXT("%s"), *__InternalMessage__) \
        REN_PRINT(__PrintMessage__, Time, Color) \
    }


#define LOG_INFO(Category, Text, ...) \
    REN_LOG(Category, Log, Text, ##__VA_ARGS__)

#define LOG_WARNING(Category, Text, ...) \
    REN_LOG(Category, Warning, Text, ##__VA_ARGS__)

#define LOG_ERROR(Category, Text, ...) \
    REN_LOG(Category, Error, Text, ##__VA_ARGS__)


#define PRINT_INFO(Category, Time, Text, ...) \
    REN_PRINT_LOG(Category, Log, Time, FColor::Green, Text, ##__VA_ARGS__)

#define PRINT_WARNING(Category, Time, Text, ...) \
    REN_PRINT_LOG(Category, Warning, Time, FColor::Orange, Text, ##__VA_ARGS__)

#define PRINT_ERROR(Category, Time, Text, ...) \
    REN_PRINT_LOG(Category, Error, Time, FColor::Red, Text, ##__VA_ARGS__)

