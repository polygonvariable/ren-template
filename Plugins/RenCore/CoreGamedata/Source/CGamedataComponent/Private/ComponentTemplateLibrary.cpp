// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "ComponentTemplateLibrary.h"


void FComponentTemplateLibrary::RegisterComponents(AActor* InActor, TArray<FComponentDefinition>& InComponents, TArray<UActorComponent*>& OutComponents)
{
	OutComponents.Reserve(InComponents.Num());

	for (FComponentDefinition& Definition : InComponents)
	{
		FComponentTemplateData* Data = Definition.Data.GetMutablePtr<FComponentTemplateData>();
		if (!Data || !Data->IsValid())
		{
			continue;
		}

		if (Definition.bModifyExisting)
		{
			UActorComponent* TargetComponent = InActor->FindComponentByClass(Data->GetComponentClass());
			if (IsValid(TargetComponent))
			{
				Data->ApplyToInstance(TargetComponent);
			}
			continue;
		}

		UActorComponent* NewComponent = NewObject<UActorComponent>(InActor, Data->GetComponentClass());
		if (IsValid(NewComponent))
		{
			NewComponent->RegisterComponent();

			Data->AttachToParent(NewComponent, InActor);
			Data->ApplyToInstance(NewComponent);

			InActor->AddInstanceComponent(NewComponent);

			OutComponents.Add(NewComponent);
		}
	}
}

void FComponentTemplateLibrary::RegisterComponents(AActor* InActor, TArray<FComponentDefinition>& InComponents)
{
	TArray<UActorComponent*> Components;
	RegisterComponents(InActor, InComponents, Components);
}

