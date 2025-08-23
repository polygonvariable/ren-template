// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EventflowEdFactory.h"

// Engine Headers

// Project Headers
#include "RenEventflow/Public/EventflowAsset.h"



UEventflowEdFactory::UEventflowEdFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    SupportedClass = UEventflowAsset::StaticClass();
}

UObject* UEventflowEdFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
    UEventflowAsset* NewAsset = NewObject<UEventflowAsset>(InParent, Class, Name, Flags);
	return NewAsset;
}

bool UEventflowEdFactory::CanCreateNew() const
{
    return true;
}
