// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers


// Project Headers
#include "Definition/TaskType.h"

// Generated Headers

// Module Macros
#define REN_API RCORESTORAGE_API

// Forward Declarations
class UStorage;



/**
 *
 * 
 *
 */
struct FStorageHandle
{

public:

    TSubclassOf<UStorage> StorageClass;
    FName StorageId;
    FString Url;
    FTaskCallback Callback;

    REN_API bool IsValid() const;
    REN_API bool IsUrlValid() const;

};



// Module Macros
#undef REN_API

