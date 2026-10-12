// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "ComponentTemplateLibrary.h"


void FComponentTemplateLibrary::RegisterComponents(AActor* InActor, TArray<TInstancedStruct<FComponentTemplateDefinition>>& InComponents, TArray<UActorComponent*>& OutComponents)
{
	OutComponents.Reserve(InComponents.Num());

	for (TInstancedStruct<FComponentTemplateDefinition>& Definition : InComponents)
	{
		FComponentTemplateDefinition* Data = Definition.GetMutablePtr<FComponentTemplateDefinition>();
		if (!Data)
		{
			continue;
		}

		UActorComponent* Component = Data->CreateComponent(InActor);
		if (IsValid(Component) && !Data->bModifyExisting)
		{
			OutComponents.Add(Component);
		}
	}
}

void FComponentTemplateLibrary::RegisterComponents(AActor* InActor, TArray<TInstancedStruct<FComponentTemplateDefinition>>& InComponents)
{
	TArray<UActorComponent*> Components;
	RegisterComponents(InActor, InComponents, Components);
}
