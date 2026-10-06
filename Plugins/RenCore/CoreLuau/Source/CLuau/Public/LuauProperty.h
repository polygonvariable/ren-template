// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "StructUtils/InstancedStruct.h"

// Generated Headers
#include "LuauProperty.generated.h"


/**
 *
 */
USTRUCT(BlueprintType)
struct FLuauProperty
{

	GENERATED_BODY()

public:

	FLuauProperty() {};

	virtual ~FLuauProperty() = default;

};


/**
 *
 */
USTRUCT(BlueprintType)
struct FLuauProperty_Nil : public FLuauProperty
{

	GENERATED_BODY()

public:

	FLuauProperty_Nil() {};

};


/**
 *
 */
USTRUCT(BlueprintType)
struct FLuauProperty_String : public FLuauProperty
{

	GENERATED_BODY()

public:

	FLuauProperty_String() {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Value = TEXT_EMPTY;

};


/**
 *
 */
USTRUCT(BlueprintType)
struct FLuauProperty_Number : public FLuauProperty
{

	GENERATED_BODY()

public:

	FLuauProperty_Number() {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double Value = 0.0f;

};


/**
 *
 */
USTRUCT(BlueprintType)
struct FLuauProperty_Boolean : public FLuauProperty
{

	GENERATED_BODY()

public:

	FLuauProperty_Boolean() {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bValue = false;

};


/**
 *
 */
USTRUCT(BlueprintType)
struct FLuauProperty_Vector : public FLuauProperty
{

	GENERATED_BODY()

public:

	FLuauProperty_Vector() {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Value = FVector();

};


/**
 *
 */
USTRUCT(BlueprintType)
struct FLuauProperty_TableEntry
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Key;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TInstancedStruct<FLuauProperty> Value;

};


/**
 *
 */
USTRUCT(BlueprintType)
struct FLuauProperty_Table : public FLuauProperty
{

	GENERATED_BODY()

public:

	FLuauProperty_Table() {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FLuauProperty_TableEntry> Value;


	void Set(const FString& Key, double Number)
	{
		FLuauProperty_TableEntry& Entry = Value.AddDefaulted_GetRef();
		Entry.Key = Key;

		FLuauProperty_Number& Property = Entry.Value.InitializeAs<FLuauProperty_Number>();
		Property.Value = Number;
	}

	void Set(const FString& Key, int Number)
	{
		Set(Key, static_cast<double>(Number));
	}

	void Set(const FString& Key, float Number)
	{
		Set(Key, static_cast<double>(Number));
	}

	void Set(const FString& Key, bool bBoolean)
	{
		FLuauProperty_TableEntry& Entry = Value.AddDefaulted_GetRef();
		Entry.Key = Key;

		FLuauProperty_Boolean& Property = Entry.Value.InitializeAs<FLuauProperty_Boolean>();
		Property.bValue = bBoolean;
	}

	void Set(const FString& Key, const FString& String)
	{
		FLuauProperty_TableEntry& Entry = Value.AddDefaulted_GetRef();
		Entry.Key = Key;

		FLuauProperty_String& Property = Entry.Value.InitializeAs<FLuauProperty_String>();
		Property.Value = String;
	}

	void Set(const FString& Key, const FVector& Vector)
	{
		FLuauProperty_TableEntry& Entry = Value.AddDefaulted_GetRef();
		Entry.Key = Key;

		FLuauProperty_Vector& Property = Entry.Value.InitializeAs<FLuauProperty_Vector>();
		Property.Value = Vector;
	}

	FLuauProperty_Table& SetTable(const FString& Key)
	{
		FLuauProperty_TableEntry& Entry = Value.AddDefaulted_GetRef();
		Entry.Key = Key;

		FLuauProperty_Table& Table = Entry.Value.InitializeAs<FLuauProperty_Table>();
		return Table;
	}

	template<typename T>
	const T* Get(const FString& Key) const
	{
		const FLuauProperty_TableEntry* Entry = Value.FindByPredicate([Key](const FLuauProperty_TableEntry& Entry) { return Entry.Key == Key; });
		if (!Entry)
		{
			return nullptr;
		}
		return Entry->Value.GetPtr<T>();
	}

};


/**
 *
 */
USTRUCT(BlueprintType)
struct FLuauProperties
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExcludeBaseStruct))
	TArray<TInstancedStruct<FLuauProperty>> Args;


	void Set(const FString& Value)
	{
		TInstancedStruct<FLuauProperty>& Property = Args.AddDefaulted_GetRef();
		FLuauProperty_String& String = Property.InitializeAs<FLuauProperty_String>();

		String.Value = Value;
	}

	void Set(float Value)
	{
		TInstancedStruct<FLuauProperty>& Property = Args.AddDefaulted_GetRef();
		FLuauProperty_Number& Number = Property.InitializeAs<FLuauProperty_Number>();

		Number.Value = Value;
	}

	void Set(double Value)
	{
		TInstancedStruct<FLuauProperty>& Property = Args.AddDefaulted_GetRef();
		FLuauProperty_Number& Number = Property.InitializeAs<FLuauProperty_Number>();

		Number.Value = Value;
	}

	void Set(int Value)
	{
		Set(static_cast<double>(Value));
	}

	void Set(bool Value)
	{
		TInstancedStruct<FLuauProperty>& Property = Args.AddDefaulted_GetRef();
		FLuauProperty_Boolean& Boolean = Property.InitializeAs<FLuauProperty_Boolean>();

		Boolean.bValue = Value;
	}

	void Set(const FVector& Value)
	{
		TInstancedStruct<FLuauProperty>& Property = Args.AddDefaulted_GetRef();
		FLuauProperty_Vector& Vector = Property.InitializeAs<FLuauProperty_Vector>();

		Vector.Value = Value;
	}

	FLuauProperty_Table& SetTable()
	{
		TInstancedStruct<FLuauProperty>& Property = Args.AddDefaulted_GetRef();
		return Property.InitializeAs<FLuauProperty_Table>();
	}

	template<typename T>
	const T* Get(int Index) const
	{
		if (!Args.IsValidIndex(Index))
		{
			return nullptr;
		}
		return Args[Index].GetPtr<T>();
	}

};

