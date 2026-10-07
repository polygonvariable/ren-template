// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Forward Declaration
struct FLuauSourceCode;


/*
 *
 */
class CLUAU_API FLuauLibrary
{

public:

	static bool Compile(const FString& InCode, TArray<uint8>& OutBytecode);
	static bool Compile(FLuauSourceCode& LuauCode);

};

