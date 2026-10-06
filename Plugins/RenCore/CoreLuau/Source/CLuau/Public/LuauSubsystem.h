// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/GameInstanceSubsystem.h"

// Project Headers
#include "lua.h"

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

	void EnsureState();
	lua_State* GetState() const;
	void CreateState();
	void CloseState();

	UFUNCTION(BlueprintCallable)
	bool CompileCode(const FString& InCode, TArray<uint8>& OutBytecode);

	UFUNCTION(BlueprintCallable)
	CLUAU_API bool ExecuteBytecode(const TArray<uint8>& Bytecode, const FString& Chunk, const FString& Method, const FLuauProperties& Input, FLuauProperties& Output);

	CLUAU_API bool ExecuteBytecodeWithContext(const TArray<uint8>& Bytecode, const FString& Chunk, const FString& Method, const FLuauProperties& Input, FLuauProperties& Output, UObject* Context);

	CLUAU_API void RegisterFunction(const FString& Namespace, const FString& FunctionName, lua_CFunction Function);

	UObject* GetCurrentContext();

protected:


	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem

private:

	UPROPERTY()
	TArray<TWeakObjectPtr<UObject>> Contexts;
	
	lua_State* L = nullptr;


	bool PushProperty(lua_State* State, const TInstancedStruct<FLuauProperty>& Property);
	bool ReadProperty(lua_State* State, int StackIndex, TInstancedStruct<FLuauProperty>& OutProperty);

	bool PushTable(lua_State* State, const FLuauProperty_Table& Table);
	bool ReadTable(lua_State* State, int StackIndex, FLuauProperty_Table& OutTable);

public:

	static CLUAU_API ULuauSubsystem* Get(UWorld* World);
	static CLUAU_API ULuauSubsystem* Get(UGameInstance* GameInstance);

	static CLUAU_API ULuauSubsystem* GetFromState(lua_State* InL);
	static CLUAU_API UObject* GetCurrentContextFromState(lua_State* LInL);

};

