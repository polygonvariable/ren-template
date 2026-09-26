// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "StructUtils/InstancedStruct.h"

// Generated Headers
#include "LuauProperty.generated.h"


/**
 *
 */
USTRUCT(BlueprintType)
struct FLuauProperty
{

	GENERATED_BODY()

public:

	FLuauProperty() {};

	virtual ~FLuauProperty() = default;

};


/**
 *
 */
USTRUCT(BlueprintType)
struct FLuauProperty_Nil : public FLuauProperty
{

	GENERATED_BODY()

public:

	FLuauProperty_Nil() {};

};


/**
 *
 */
USTRUCT(BlueprintType)
struct FLuauProperty_String : public FLuauProperty
{

	GENERATED_BODY()

public:

	FLuauProperty_String() {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Value = TEXT_EMPTY;

};


/**
 *
 */
USTRUCT(BlueprintType)
struct FLuauProperty_Number : public FLuauProperty
{

	GENERATED_BODY()

public:

	FLuauProperty_Number() {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double Value = 0.0f;

};


/**
 *
 */
USTRUCT(BlueprintType)
struct FLuauProperty_Boolean : public FLuauProperty
{

	GENERATED_BODY()

public:

	FLuauProperty_Boolean() {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bValue = false;

};


/**
 *
 */
USTRUCT(BlueprintType)
struct FLuauProperty_Vector : public FLuauProperty
{

	GENERATED_BODY()

public:

	FLuauProperty_Vector() {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Value = FVector();

};


/**
 *
 */
USTRUCT(BlueprintType)
struct FLuauProperty_TableEntry
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Key;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TInstancedStruct<FLuauProperty> Value;

};


/**
 *
 */
USTRUCT(BlueprintType)
struct FLuauProperty_Table : public FLuauProperty
{

	GENERATED_BODY()

public:

	FLuauProperty_Table() {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FLuauProperty_TableEntry> Value;

};


/**
 *
 */
USTRUCT(BlueprintType)
struct FLuauProperties
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExcludeBaseStruct))
	TArray<TInstancedStruct<FLuauProperty>> Args;

};

