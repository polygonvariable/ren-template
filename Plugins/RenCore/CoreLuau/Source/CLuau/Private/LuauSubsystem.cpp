// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "LuauSubsystem.h"

// Project Headers
#include "lua.h"
#include "luacode.h"
#include "lualib.h"
#include "LuauProperty.h"
#include "LuauLibrary.h"


lua_State* ULuauSubsystem::GetState() const
{
	return L;
}

void ULuauSubsystem::CreateState()
{
	if (!L)
	{
		L = luaL_newstate();
		luaL_openlibs(L);
	}
}

void ULuauSubsystem::CloseState()
{
	lua_close(L);
}


bool ULuauSubsystem::CompileCode(const FString& InCode, TArray<uint8>& OutBytecode)
{
	return FLuauHelper::Compile(InCode, OutBytecode);
}

bool ULuauSubsystem::ExecuteBytecode(const TArray<uint8>& Bytecode, const FString& Chunk, const FString& Method, const FLuauProperties& Input, FLuauProperties& Output)
{
	int BytecodeSize = Bytecode.Num();
	if (!L || BytecodeSize == 0 || Chunk.IsEmpty() || Method.IsEmpty())
	{
		return false;
	}

	const char* LData = reinterpret_cast<const char*>(Bytecode.GetData());
	size_t LSize = BytecodeSize;

	const char* LChunk = TCHAR_TO_UTF8(*Chunk);
	const char* LMethod = TCHAR_TO_UTF8(*Method);

	int Status = luau_load(L, LChunk, LData, LSize, 0);
	if (Status != LUA_OK)
	{
		const char* Message = lua_tostring(L, -1);
		UE_LOG(LogTemp, Error, TEXT("Luau load error: %s"), Message ? UTF8_TO_TCHAR(Message) : TEXT("Unknown error"));

		lua_pop(L, 1);
		return false;
	}

	if (lua_pcall(L, 0, 0, 0) != LUA_OK)
	{
		const char* Message = lua_tostring(L, -1);
		UE_LOG(LogTemp, Error, TEXT("Luau chunk error: %s"), Message ? UTF8_TO_TCHAR(Message) : TEXT("Unknown error"));

		lua_pop(L, 1);
		return false;
	}
	
	lua_getglobal(L, LMethod);
	if (!lua_isfunction(L, -1))
	{
		UE_LOG(LogTemp, Error, TEXT("Luau method '%s' is not a function"), *Method);

		lua_pop(L, 1);
		return false;
	}

	int InputNum = Input.Args.Num();
	for (const TInstancedStruct<FLuauProperty>& Property : Input.Args)
	{
		if (!PushProperty(L, Property))
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to push Luau parameter"));

			lua_pop(L, InputNum + 1);
			return false;
		}
	}

	if (lua_pcall(L, InputNum, LUA_MULTRET, 0) != LUA_OK)
	{
		const char* Message = lua_tostring(L, -1);
		UE_LOG(LogTemp, Error, TEXT("Luau method error: %s"), Message ? UTF8_TO_TCHAR(Message) : TEXT("Unknown error"));

		lua_pop(L, 1);
		return false;
	}

	const int OutputCount = lua_gettop(L);
	Output.Args.Reset(OutputCount);

	for (int Index = 1; Index <= OutputCount; ++Index)
	{
		TInstancedStruct<FLuauProperty> Property;

		if (!ReadProperty(L, Index, Property))
		{
			UE_LOG(LogTemp, Error, TEXT("Unsupported Luau return value at index %d"), Index);

			lua_settop(L, 0);
			return false;
		}

		Output.Args.Add(MoveTemp(Property));
	}

	lua_settop(L, 0);
	return true;
}


bool ULuauSubsystem::PushProperty(lua_State* State, const TInstancedStruct<FLuauProperty>& Property)
{
	if (const FLuauProperty_String* Value = Property.GetPtr<FLuauProperty_String>())
	{
		lua_pushstring(State, TCHAR_TO_UTF8(*Value->Value));
		return true;
	}

	if (const FLuauProperty_Number* Value = Property.GetPtr<FLuauProperty_Number>())
	{
		lua_pushnumber(State, Value->Value);
		return true;
	}

	if (const FLuauProperty_Boolean* Value = Property.GetPtr<FLuauProperty_Boolean>())
	{
		lua_pushboolean(State, Value->bValue);
		return true;
	}

	if (const FLuauProperty_Vector* Value = Property.GetPtr<FLuauProperty_Vector>())
	{
		lua_pushvector(State, Value->Value.X, Value->Value.Y, Value->Value.Z);
		return true;
	}

	if (const FLuauProperty_Table* Value = Property.GetPtr<FLuauProperty_Table>())
	{
		return PushTable(State, *Value);
	}

	if (Property.GetPtr<FLuauProperty_Nil>())
	{
		lua_pushnil(State);
		return true;
	}

	return false;
}

bool ULuauSubsystem::ReadProperty(lua_State* State, int StackIndex, TInstancedStruct<FLuauProperty>& OutProperty)
{
	const int Type = lua_type(State, StackIndex);

	switch (Type)
	{
	case LUA_TNIL:
	{
		OutProperty.InitializeAs<FLuauProperty_Nil>();
		return true;
	}

	case LUA_TSTRING:
	{
		const char* Value = lua_tostring(State, StackIndex);
		FLuauProperty_String& Property = OutProperty.InitializeAs<FLuauProperty_String>();
		Property.Value = UTF8_TO_TCHAR(Value);
		return true;
	}

	case LUA_TNUMBER:
	{
		const double Value = lua_tonumber(State, StackIndex);
		FLuauProperty_Number& Property = OutProperty.InitializeAs<FLuauProperty_Number>();
		Property.Value = static_cast<float>(Value);
		return true;
	}

	case LUA_TBOOLEAN:
	{
		FLuauProperty_Boolean& Property = OutProperty.InitializeAs<FLuauProperty_Boolean>();
		Property.bValue = lua_toboolean(State, StackIndex) != 0;
		return true;
	}
	
	case LUA_TVECTOR:
	{
		const float* Value = lua_tovector(State, StackIndex);
		if (!Value)
		{
			return false;
		}
		FLuauProperty_Vector& Property = OutProperty.InitializeAs<FLuauProperty_Vector>();
		Property.Value = FVector(Value[0], Value[1], Value[2]);
		return true;
	}

	case LUA_TTABLE:
	{
		FLuauProperty_Table& Property = OutProperty.InitializeAs<FLuauProperty_Table>();
		return ReadTable(State, StackIndex, Property);
	}

	default:
		return false;
	}
}

bool ULuauSubsystem::PushTable(lua_State* State, const FLuauProperty_Table& Table)
{
	lua_createtable(State, 0, Table.Value.Num());

	for (const FLuauProperty_TableEntry& Entry : Table.Value)
	{
		lua_pushstring(State, TCHAR_TO_UTF8(*Entry.Key));

		if (!Entry.Value.IsValid() || !PushProperty(State, Entry.Value))
		{
			lua_pop(State, 1);
			lua_pop(State, 1);
			return false;
		}

		lua_settable(State, -3);
	}

	return true;
}

bool ULuauSubsystem::ReadTable(lua_State* State, int StackIndex, FLuauProperty_Table& OutTable)
{
	StackIndex = lua_absindex(State, StackIndex);
	OutTable.Value.Reset();

	lua_pushnil(State);

	while (lua_next(State, StackIndex) != 0)
	{
		if (lua_type(State, -2) != LUA_TSTRING)
		{
			UE_LOG(LogTemp, Error, TEXT("Luau table contains a non-string key"));

			lua_pop(State, 1);
			return false;
		}

		const char* Key = lua_tostring(State, -2);

		if (!Key)
		{
			lua_pop(State, 1);
			return false;
		}

		FLuauProperty_TableEntry& Entry = OutTable.Value.AddDefaulted_GetRef();
		Entry.Key = UTF8_TO_TCHAR(Key);

		if (!ReadProperty(State, -1, Entry.Value))
		{
			lua_pop(State, 1);

			OutTable.Value.Pop();
			return false;
		}

		lua_pop(State, 1);
	}

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

