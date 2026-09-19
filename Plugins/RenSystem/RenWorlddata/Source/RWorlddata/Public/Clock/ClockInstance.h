// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Generated Headers
#include "ClockInstance.generated.h"


/**
 * 
 */
USTRUCT()
struct FClockInstance
{

	GENERATED_BODY()

public:

	FClockInstance() {};
	FClockInstance(int InTime, int InDay, int InYear) : Time(InTime), Day(InDay), Year(InYear) {};

	UPROPERTY(EditAnywhere, SaveGame, meta = (UIMin = "1", ClampMin = "1"))
	int Time = 1;

	UPROPERTY(EditAnywhere, SaveGame, meta = (UIMin = "1", ClampMin = "1"))
	int Day = 1;

	UPROPERTY(EditAnywhere, SaveGame, meta = (UIMin = "1", ClampMin = "1"))
	int Year = 1;

	void Reset()
	{
		Time = 1;
		Day = 1;
		Year = 1;
	}

	void Clamp(int DayLength, int YearLength)
	{
		if (DayLength <= 0 || YearLength <= 0)
		{
			Reset();
			return;
		}

		Time = FMath::Max(1, Time);
		Day = FMath::Max(1, Day);
		Year = FMath::Max(1, Year);

		if (Time > DayLength)
		{
			const int ExtraDays = (Time - 1) / DayLength;

			Time = ((Time - 1) % DayLength) + 1;
			Day += ExtraDays;
		}

		if (Day > YearLength)
		{
			const int ExtraYears = (Day - 1) / YearLength;

			Day = ((Day - 1) % YearLength) + 1;
			Year += ExtraYears;
		}
	}

	void AddSeconds(int Seconds, int DayLength, int YearLength)
	{
		Time += Seconds;
		Clamp(DayLength, YearLength);
	}

	void AddDays(int Days, int DayLength, int YearLength)
	{
		Day += Days;
		Clamp(DayLength, YearLength);
	}

	void AddYears(int Years, int DayLength, int YearLength)
	{
		Year += Years;
		Clamp(DayLength, YearLength);
	}

	friend inline bool operator < (const FClockInstance& A, const FClockInstance& B)
	{
		if (A.Year != B.Year)
		{
			return A.Year < B.Year;
		}

		if (A.Day != B.Day)
		{
			return A.Day < B.Day;
		}

		return A.Time < B.Time;
	}

	friend inline bool operator > (const FClockInstance& A, const FClockInstance& B)
	{
		return B < A;
	}

	friend inline bool operator <= (const FClockInstance& A, const FClockInstance& B)
	{
		return !(B < A);
	}

	friend inline bool operator >= (const FClockInstance& A, const FClockInstance& B)
	{
		return !(A < B);
	}

	friend inline bool operator == (const FClockInstance& A, const FClockInstance& B)
	{
		return A.Time == B.Time && A.Day == B.Day && A.Year == B.Year;
	}

	friend inline uint32 GetTypeHash(const FClockInstance& A)
	{
		uint32 Hash = GetTypeHash(A.Time);
		Hash = HashCombineFast(Hash, GetTypeHash(A.Day));
		Hash = HashCombineFast(Hash, GetTypeHash(A.Year));
		return Hash;
	}

};

