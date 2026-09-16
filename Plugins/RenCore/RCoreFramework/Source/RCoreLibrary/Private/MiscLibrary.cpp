// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "MiscLibrary.h"


float FMiscLibrary::PackFloats(float ValueA, float ValueB)
{
    FFloat16 A(ValueA);
    FFloat16 B(ValueB);

    uint32 PackedInt = (uint32(A.Encoded) << 16) | uint32(B.Encoded);

    float PackedFloat;
    FMemory::Memcpy(&PackedFloat, &PackedInt, sizeof(float));

    return PackedFloat;
}

UWorld* FMiscLibrary::GetCurrentWorld()
{
	if (GEngine)
	{
		for (const FWorldContext& Context : GEngine->GetWorldContexts())
		{
			if (Context.WorldType == EWorldType::Game || Context.WorldType == EWorldType::PIE)
			{
				return Context.World();
			}
		}
	}
	return nullptr;
}

