// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/*
 *
 */
namespace FMiscLibrary
{

	CLIBRARY_API float PackFloats(float ValueA, float ValueB);
	CLIBRARY_API UWorld* GetCurrentWorld();

    template <typename UserClass>
    void NextFrame(UObject* Target, void(UserClass::* Method)())
    {
        check(Target);

        UWorld* World = Target->GetWorld();
        check(World);

        World->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(Target,
            [Target, Method]()
            {
                UserClass* TargetClass = Cast<UserClass>(Target);
                if (IsValid(TargetClass))
                {
                    (TargetClass->*Method)();
                }
            })
        );
    }

};

