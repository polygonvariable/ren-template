// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Forward Declaratoin
struct lua_State;


/*
 *
 */
class CLUAU_API FLuauHelper
{

public:

	static bool Compile(const FString& InCode, TArray<uint8>& OutBytecode);

};

