// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DataAsset.h"
#include "StructUtils/InstancedStruct.h"

// Generated Headers
#include "LuauFragment.generated.h"

// Module Macros
#define REN_API RCORELUAU_API

struct lua_State;



class REN_API FLuauHelper
{

public:

	static bool Compile(const FString& InCode, TArray<uint8>& OutBytecode);

};


UENUM(BlueprintType)
enum class ELuauType : uint8
{
	NIL UMETA(DisplayName = "Nil"),
	STRING UMETA(DisplayName = "String"),
	NUMBER UMETA(DisplayName = "Number"),
	BOOLEAN UMETA(DisplayName = "Boolean"),
	TABLE UMETA(DisplayName = "Table"),
	VECTOR UMETA(DisplayName = "Vector"),
	USERDATA UMETA(DisplayName = "User Data"),
};


template<typename T>
struct H
{
	T SomeData;

	T GetData() const
	{
		return SomeData;
	}

	void SetData(T InData)
	{
		T = InData;
	}
};


USTRUCT(BlueprintType)
struct FLuauProperty
{

	GENERATED_BODY()

};

USTRUCT(BlueprintType)
struct FLuauProperty_String : public FLuauProperty
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FString Value = TEXT_EMPTY;

};

USTRUCT(BlueprintType)
struct FLuauProperty_Number : public FLuauProperty
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	float Value = 0.0f;

};

USTRUCT(BlueprintType)
struct FLuauProperty_Boolean : public FLuauProperty
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	bool bValue = false;

};





USTRUCT(BlueprintType)
struct FLuauParameters
{
	
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, meta = (BaseStruct = "/Script/RCoreLuau.LuauProperty", ExcludeBaseStruct))
	TArray<FInstancedStruct> Args;

};








USTRUCT(BlueprintType)
struct FLuauBytecode
{

	GENERATED_BODY()

public:

	UPROPERTY()
	TArray<uint8> Bytecode;

};


USTRUCT(BlueprintType)
struct FLuauSourceCode
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FString Code;

	UPROPERTY(VisibleAnywhere)
	TArray<uint8> Bytecode;

	const FString& Get() const
	{
		return Code;
	}

	bool IsValid() const
	{
		return Code.Len() > 0;
	}

};






UCLASS(MinimalAPI)
class ULuauAsset : public UPrimaryDataAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FLuauSourceCode SourceCode;

};







// Module Macros
#undef REN_API

