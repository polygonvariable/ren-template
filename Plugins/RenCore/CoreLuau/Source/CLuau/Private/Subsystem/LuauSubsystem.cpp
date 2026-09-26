// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Subsystem/LuauSubsystem.h"

// Project Headers
#include "lua.h"
#include "lualib.h"
#include "luacode.h"


void ULuauSubsystem::CreateState()
{
	if (!L)
	{
		L = luaL_newstate();
		luaL_openlibs(L);
	}
}

lua_State* ULuauSubsystem::GetState() const
{
	return L;
}

void ULuauSubsystem::CloseState()
{
	lua_close(L);
}

bool ULuauSubsystem::CompileCode(const FString& InCode, TArray<uint8>& OutBytecode)
{
	return FLuauHelper::Compile(InCode, OutBytecode);
}

bool ULuauSubsystem::ExecuteBytecode(const TArray<uint8>& Bytecode, const FString& Chunk, const FString& Method, const FLuauParameters& Input, const FLuauParameters& Output)
{
	int BytecodeSize = Bytecode.Num();
	if (!L || BytecodeSize == 0 || Chunk.IsEmpty() || Method.IsEmpty())
	{
		return false;
	}

	const char* LData = reinterpret_cast<const char*>(Bytecode.GetData());
	size_t LSize = BytecodeSize;

	const char* LChunk = TCHAR_TO_ANSI(*Chunk);
	const char* LMethod = TCHAR_TO_ANSI(*Method);

	int Status = luau_load(L, LChunk, LData, LSize, 0);
	if (Status != LUA_OK)
	{
		const char* Message = lua_tostring(L, -1);
		UE_LOG(LogTemp, Error, TEXT("%s"), UTF8_TO_TCHAR(Message));

		lua_pop(L, 1);
		return false;
	}

	if (lua_pcall(L, 0, 0, 0) != LUA_OK)
	{
		const char* Message = lua_tostring(L, -1);
		UE_LOG(LogTemp, Error, TEXT("%s"), UTF8_TO_TCHAR(Message));

		lua_pop(L, 1);
		return false;
	}
	
	lua_getglobal(L, LMethod);


	const TArray<FInstancedStruct>& Inputs = Input.Args;
	int InputNum = Inputs.Num();

	const TArray<FInstancedStruct>& Outputs = Output.Args;
	int OutputNum = Outputs.Num();


	for (const FInstancedStruct& Arg : Inputs)
	{
		if (const FLuauProperty_String* PropertyString = Arg.GetPtr<FLuauProperty_String>())
		{
			lua_pushstring(L, TCHAR_TO_ANSI(*PropertyString->Value));
		}
		else if (const FLuauProperty_Number* PropertyNumber = Arg.GetPtr<FLuauProperty_Number>())
		{
			lua_pushnumber(L, PropertyNumber->Value);
		}
		else if (const FLuauProperty_Boolean* PropertyBoolean = Arg.GetPtr<FLuauProperty_Boolean>())
		{
			lua_pushboolean(L, PropertyBoolean->bValue);
		}
	}

	if (lua_pcall(L, InputNum, OutputNum, 0) != LUA_OK)
	{
		const char* Message = lua_tostring(L, -1);
		UE_LOG(LogTemp, Error, TEXT("%s"), UTF8_TO_TCHAR(Message));

		lua_pop(L, 1);
		return false;
	}

	double result = lua_tonumber(L, -1);
	UE_LOG(LogTemp, Error, TEXT("reult: %d"), static_cast<int>(result));

	lua_pop(L, 1);

	return true;
}








bool ULuauSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void ULuauSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	CreateState();
}

void ULuauSubsystem::Deinitialize()
{
	CloseState();
	Super::Deinitialize();
}

ULuauSubsystem* ULuauSubsystem::Get(UWorld* World)
{
	if (!IsValid(World))
	{
		return nullptr;
	}
	return Get(World->GetGameInstance());
}

ULuauSubsystem* ULuauSubsystem::Get(UGameInstance* GameInstance)
{
	if (!IsValid(GameInstance))
	{
		return nullptr;
	}
	return GameInstance->GetSubsystem<ULuauSubsystem>();
}

