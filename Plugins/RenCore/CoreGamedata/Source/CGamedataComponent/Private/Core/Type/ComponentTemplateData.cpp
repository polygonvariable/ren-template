// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Core/Type/ComponentTemplateData.h"


bool FComponentTemplateData::IsDataValid(UClass* Target) const
{
    return false;
}

bool FComponentTemplateData::AttachToParent(UActorComponent* Target, AActor* Owner)
{
    return true;
};

bool FComponentTemplateData::ApplyToInstance(UObject* Target)
{
    return false;
};


bool FSceneComponentTemplateData::AttachToParent(UActorComponent* Target, AActor* Owner)
{
    USceneComponent* Component = Cast<USceneComponent>(Target);
    if (!IsValid(Component) || !NativeParent.IsValid())
    {
        return false;
    }
    Component->AttachToComponent(Owner->FindComponentByTag<USceneComponent>(NativeParent), FAttachmentTransformRules::KeepRelativeTransform, NativeParentSocket);
    return true;
};

