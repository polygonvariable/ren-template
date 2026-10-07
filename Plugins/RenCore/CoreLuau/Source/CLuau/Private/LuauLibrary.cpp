// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "LuauLibrary.h"

// Project Headers
#include "LuauSourceCode.h"
#include "luacode.h"


bool FLuauLibrary::Compile(const FString& InCode, TArray<uint8>& OutBytecode)
{
    bool bResult = false;
    OutBytecode.Empty();

    if (InCode.IsEmpty())
    {
        return bResult;
    }

    FTCHARToUTF8 Code(*InCode);

    size_t BytecodeSize = 0;
    char* Bytecode = luau_compile(Code.Get(), Code.Length(), nullptr, &BytecodeSize);
    if (Bytecode && BytecodeSize > 0)
    {
        if (Bytecode[0] != 0)
        {
            OutBytecode.SetNum(BytecodeSize);
            FMemory::Memcpy(OutBytecode.GetData(), Bytecode, BytecodeSize);

            bResult = true;
        }
    }

    free(Bytecode);

    return bResult;
}

bool FLuauLibrary::Compile(FLuauSourceCode& LuauCode)
{
    FString LibraryCode;
    FString GeneratedCode;

    if (LuauCode.Library.IsEmpty() || !FFileHelper::LoadFileToString(LibraryCode, *LuauCode.Library))
    {
        GeneratedCode = LuauCode.Code;
        UE_LOG(LogTemp, Warning, TEXT("Luau library path is empty or invalid"));
    }
    else
    {
        GeneratedCode = LibraryCode + "\n\n" + LuauCode.Code;
        UE_LOG(LogTemp, Log, TEXT("Luau generated code:\n%s"), *GeneratedCode);
    }

    return FLuauLibrary::Compile(GeneratedCode, LuauCode.Bytecode);
}

