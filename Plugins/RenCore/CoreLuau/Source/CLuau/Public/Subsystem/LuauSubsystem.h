// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/GameInstanceSubsystem.h"

// Project Headers
#include "LuauFragment.h"

// Generated Headers
#include "LuauSubsystem.generated.h"

struct lua_State;

/**
 * 
 */
UCLASS(MinimalAPI)
class ULuauSubsystem : public UGameInstanceSubsystem
{

	GENERATED_BODY()

public:

	void CreateState();
	lua_State* GetState() const;
	void CloseState();

	UFUNCTION(BlueprintCallable)
	bool CompileCode(const FString& InCode, TArray<uint8>& OutBytecode);

	UFUNCTION(BlueprintCallable)
	bool ExecuteBytecode(const TArray<uint8>& Bytecode, const FString& Chunk, const FString& Method, const FLuauParameters& Input, const FLuauParameters& Output);

protected:

	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem

private:

	lua_State* L = nullptr;

public:

	static ULuauSubsystem* Get(UWorld* World);
	static ULuauSubsystem* Get(UGameInstance* GameInstance);

};

