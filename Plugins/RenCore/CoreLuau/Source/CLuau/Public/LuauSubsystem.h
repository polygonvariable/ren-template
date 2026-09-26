// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/GameInstanceSubsystem.h"

// Generated Headers
#include "LuauSubsystem.generated.h"

// Forward Declaration
struct lua_State;
struct FLuauProperty;
struct FLuauProperty_Table;
struct FLuauProperties;


/**
 * 
 */
UCLASS(MinimalAPI)
class ULuauSubsystem : public UGameInstanceSubsystem
{

	GENERATED_BODY()

public:

	lua_State* GetState() const;
	void CreateState();
	void CloseState();

	UFUNCTION(BlueprintCallable)
	bool CompileCode(const FString& InCode, TArray<uint8>& OutBytecode);

	UFUNCTION(BlueprintCallable)
	bool ExecuteBytecode(const TArray<uint8>& Bytecode, const FString& Chunk, const FString& Method, const FLuauProperties& Input, FLuauProperties& Output);

protected:

	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem

private:

	lua_State* L = nullptr;


	bool PushProperty(lua_State* State, const TInstancedStruct<FLuauProperty>& Property);
	bool ReadProperty(lua_State* State, int StackIndex, TInstancedStruct<FLuauProperty>& OutProperty);

	bool PushTable(lua_State* State, const FLuauProperty_Table& Table);
	bool ReadTable(lua_State* State, int StackIndex, FLuauProperty_Table& OutTable);

public:

	static ULuauSubsystem* Get(UWorld* World);
	static ULuauSubsystem* Get(UGameInstance* GameInstance);

};

