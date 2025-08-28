// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "StoryboardEdFactory.h"

// Engine Headers

// Project Headers
#include "RenQuest/Public/StoryboardAsset.h"



UStoryboardEdFactory::UStoryboardEdFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    SupportedClass = UStoryboardGraph::StaticClass();
}

UObject* UStoryboardEdFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
    UStoryboardGraph* NewAsset = NewObject<UStoryboardGraph>(InParent, Class, Name, Flags);
	return NewAsset;
}

bool UStoryboardEdFactory::CanCreateNew() const
{
    return true;
}

