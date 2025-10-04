// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers

// Generated Headers

// Forward Declarations



#if WITH_EDITOR

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
		REN_LOG(Category, Verbosity, Text, ##__VA_ARGS__) \
        const FString __PrintMessage__ = FString::Printf(Text, ##__VA_ARGS__); \
        REN_PRINT(__PrintMessage__, Time, Color) \
    }

#else

#define REN_LOG(Category, Verbosity, Text, ...) \
    UE_LOG(Category, Verbosity, Text, ##__VA_ARGS__)

#define REN_PRINT_LOG(Category, Verbosity, Time, Color, Text, ...) \
    REN_LOG(Category, Verbosity, Text, ##__VA_ARGS__)

#endif

#define LOG_INFO(Category, Text, ...) \
    REN_LOG(Category, Log, Text, ##__VA_ARGS__)

#define LOG_WARNING(Category, Text, ...) \
    REN_LOG(Category, Warning, Text, ##__VA_ARGS__)

#define LOG_ERROR(Category, Text, ...) \
    REN_LOG(Category, Error, Text, ##__VA_ARGS__)


#define PRINT_SUCCESS(Category, Time, Text, ...) \
    REN_PRINT_LOG(Category, Log, Time, FColor::Green, Text, ##__VA_ARGS__)

#define PRINT_INFO(Category, Time, Text, ...) \
    REN_PRINT_LOG(Category, Log, Time, FColor::Magenta, Text, ##__VA_ARGS__)

#define PRINT_WARNING(Category, Time, Text, ...) \
    REN_PRINT_LOG(Category, Warning, Time, FColor::Orange, Text, ##__VA_ARGS__)

#define PRINT_ERROR(Category, Time, Text, ...) \
    REN_PRINT_LOG(Category, Error, Time, FColor::Red, Text, ##__VA_ARGS__)


#define TIMER_START(Name) \
	double __##Name##_StartTime__ = FPlatformTime::Seconds();

#define TIMER_END(Name, Time, Message) \
    { \
	    double __##Name##_EndTime__ = FPlatformTime::Seconds(); \
        double __##Name##_Duration__ = __##Name##_EndTime__ - __##Name##_StartTime__; \
        PRINT_WARNING(LogTemp, Time, TEXT("%s: %f"), Message, __##Name##_Duration__); \
    }

