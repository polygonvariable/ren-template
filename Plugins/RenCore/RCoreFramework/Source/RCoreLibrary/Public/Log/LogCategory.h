// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers

// Generated Headers

// Module Macros
#define REN_API RCORELIBRARY_API

// Forward Declarations


REN_API DECLARE_LOG_CATEGORY_EXTERN(LogAscension, Log, All);

// ~ Storage
REN_API DECLARE_LOG_CATEGORY_EXTERN(LogStorage, Log, All);
// ~ End Storage

// ~ Task Categories
REN_API DECLARE_LOG_CATEGORY_EXTERN(LogTask, Log, All);
REN_API DECLARE_LOG_CATEGORY_EXTERN(LogAsset, Log, All);
// ~ End of Task Categories

// ~ Library Categories
REN_API DECLARE_LOG_CATEGORY_EXTERN(LogTimer, Log, All);
// ~ End of Library Categories

// ~ Priority Categories
REN_API DECLARE_LOG_CATEGORY_EXTERN(LogPriorityList, Log, All);
// ~ End of Priority Categories

// ~ Environment Categories
REN_API DECLARE_LOG_CATEGORY_EXTERN(LogEnvironment, Log, All);
REN_API DECLARE_LOG_CATEGORY_EXTERN(LogWeather, Log, All);
REN_API DECLARE_LOG_CATEGORY_EXTERN(LogSeason, Log, All);
// ~ End of Environment Categories

// ~ Pool Categories
REN_API DECLARE_LOG_CATEGORY_EXTERN(LogPool, Log, All);
// ~ End of Pool Categories

// ~ Counter Categories
REN_API DECLARE_LOG_CATEGORY_EXTERN(LogCounter, Log, All);
REN_API DECLARE_LOG_CATEGORY_EXTERN(LogTimestamp, Log, All);
// ~ End of Counter Categories

// ~ Inventory Categories
REN_API DECLARE_LOG_CATEGORY_EXTERN(LogTrade, Log, All);
REN_API DECLARE_LOG_CATEGORY_EXTERN(LogCraft, Log, All);
REN_API DECLARE_LOG_CATEGORY_EXTERN(LogShop, Log, All);

REN_API DECLARE_LOG_CATEGORY_EXTERN(LogInventory, Log, All);
REN_API DECLARE_LOG_CATEGORY_EXTERN(LogInventoryAscension, Log, All);
REN_API DECLARE_LOG_CATEGORY_EXTERN(LogInventoryInstance, Log, All);
// ~ End of Inventory Categories

// ~ Avatar Categories
REN_API DECLARE_LOG_CATEGORY_EXTERN(LogAvatar, Log, All);
REN_API DECLARE_LOG_CATEGORY_EXTERN(LogAvatarAscension, Log, All);
REN_API DECLARE_LOG_CATEGORY_EXTERN(LogCharacterParty, Log, All);
// ~ End of Avatar Categories

// ~ Equipment Categories
REN_API DECLARE_LOG_CATEGORY_EXTERN(LogEquipment, Log, All);
// ~ End of Equipment Categories




// Module Macros
#undef REN_API

