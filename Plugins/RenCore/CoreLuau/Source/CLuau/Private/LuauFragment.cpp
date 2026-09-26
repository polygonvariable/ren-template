// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "LuauFragment.h"

// Project Headers
#include "luacode.h"


bool FLuauHelper::Compile(const FString& InCode, TArray<uint8>& OutBytecode)
{
    bool bResult = false;
    OutBytecode.Empty();

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

