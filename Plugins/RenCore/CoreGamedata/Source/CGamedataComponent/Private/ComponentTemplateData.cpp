// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "ComponentTemplateData.h"


bool FComponentTemplateData::IsValid() const
{
    return ::IsValid(GetComponentClass());
}

bool FComponentTemplateData::AttachToParent(UActorComponent* Target, AActor* Owner)
{
    return true;
}

bool FComponentTemplateData::ApplyToInstance(UObject* Target)
{
    return false;
}
 
TSubclassOf<UActorComponent> FComponentTemplateData::GetComponentClass() const
{
    return nullptr;
}


bool FSceneComponentTemplateData::AttachToParent(UActorComponent* Target, AActor* Owner)
{
    USceneComponent* Component = Cast<USceneComponent>(Target);
    if (!::IsValid(Component) || !NativeParent.IsValid())
    {
        return false;
    }
    Component->AttachToComponent(Owner->FindComponentByTag<USceneComponent>(NativeParent), FAttachmentTransformRules::KeepRelativeTransform, NativeParentSocket);
    return true;
};

