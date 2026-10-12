// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "ComponentTemplateDefinition.h"


UActorComponent* FComponentTemplateDefinition::CreateComponent(AActor* Owner)
{
    UClass* ComponentClass = GetComponentClass();
    checkf(::IsValid(ComponentClass), TEXT("Component template contains invalid component class"));
    checkf(::IsValid(Owner), TEXT("Component template is provided with invalid owner actor"));

    if (bModifyExisting)
    {
        UActorComponent* TargetComponent = Owner->FindComponentByClass(ComponentClass);
        if (!::IsValid(TargetComponent))
        {
            ensureMsgf(false, TEXT("Failed to find component to modify"));
            return nullptr;
        }

        InitializeInstance(TargetComponent);
        return TargetComponent;
    }

    UActorComponent* NewComponent = NewObject<UActorComponent>(Owner, ComponentClass);
    if (!::IsValid(NewComponent))
    {
        ensureMsgf(false, TEXT("Failed to create component"));
        return nullptr;
    }

    NewComponent->RegisterComponent();

    AttachToParent(NewComponent, Owner);
    InitializeInstance(NewComponent);

    Owner->AddInstanceComponent(NewComponent);
    return NewComponent;
}

bool FComponentTemplateDefinition::AttachToParent(UActorComponent* Target, AActor* Owner)
{
    return true;
}

void FComponentTemplateDefinition::InitializeInstance(UActorComponent* Target)
{

}
 
TSubclassOf<UActorComponent> FComponentTemplateDefinition::GetComponentClass() const
{
    return nullptr;
}

#if WITH_EDITOR
bool FComponentTemplateDefinition::IsDataValid() const
{
    return ::IsValid(GetComponentClass());
}
void FComponentTemplateDefinition::AppendAssetBundleData(FAssetBundleData& AssetBundle, const FName& Bundle)
{
}
#endif


bool FSceneComponentTemplateDefinition::IsDataValid() const
{
    if (!FComponentTemplateDefinition::IsDataValid())
    {
        return false;
    }
    return NativeParent.IsValid();
}

TSubclassOf<UActorComponent> FSceneComponentTemplateDefinition::GetComponentClass() const
{
    return USceneComponent::StaticClass();
}

bool FSceneComponentTemplateDefinition::AttachToParent(UActorComponent* Target, AActor* Owner)
{
    USceneComponent* Component = Cast<USceneComponent>(Target);
    if (!::IsValid(Component) || !NativeParent.IsValid())
    {
        return false;
    }
    Component->AttachToComponent(Owner->FindComponentByTag<USceneComponent>(NativeParent), FAttachmentTransformRules::KeepRelativeTransform, NativeParentSocket);
    return true;
};

