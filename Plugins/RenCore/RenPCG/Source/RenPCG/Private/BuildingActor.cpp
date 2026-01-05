// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "BuildingActor.h"

// Engine Headers
#include "Components/SplineComponent.h"
#include "Components/InstancedStaticMeshComponent.h"

// Project Headers




ABuildingActor::ABuildingActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));

	if (IsValid(SceneComponent))
	{
		SetRootComponent(SceneComponent);
	}
}



void ABuildingActor::GetAllStaticMeshes(TSet<UStaticMesh*>& OutMeshes) const
{
	TSet<UStaticMesh*> Meshes;
	for (const FBuildingData& Data : BuildingData)
	{
		UStaticMesh* Mesh = Data.StaticMesh;
		if (IsValid(Mesh) && !Meshes.Contains(Mesh))
		{
			Meshes.Add(Mesh);
		}
	}
	OutMeshes = Meshes;
}


void ABuildingActor::MakeISMComponents()
{
	TSet<UStaticMesh*> StaticMeshes;
	GetAllStaticMeshes(StaticMeshes);

	for (UStaticMesh* StaticMesh : StaticMeshes)
	{
		if (!IsValid(StaticMesh))
		{
			continue;
		}

		UInstancedStaticMeshComponent* Component = NewObject<UInstancedStaticMeshComponent>(this, UInstancedStaticMeshComponent::StaticClass(), NAME_None, RF_Transactional);
		if (!IsValid(Component))
		{
			return;
		}

		AddInstanceComponent(Component);
		Component->AttachToComponent(SceneComponent, FAttachmentTransformRules::KeepRelativeTransform);
		Component->SetStaticMesh(StaticMesh);
	}
}



void ABuildingActor::GetISMComponents(TArray<UInstancedStaticMeshComponent*>& OutISMComponents) const
{
	TArray<UInstancedStaticMeshComponent*> ISMComponents;

	const TSet<UActorComponent*>& Components = GetComponents();
	for (UActorComponent* Component : Components)
	{
		UInstancedStaticMeshComponent* ISM = Cast<UInstancedStaticMeshComponent>(Component);
		if (IsValid(ISM))
		{
			ISMComponents.Add(ISM);
		}
	}

	OutISMComponents = ISMComponents;
}

void ABuildingActor::DestroyISMComponents()
{
	TArray<UInstancedStaticMeshComponent*> ISMComponents;
	GetISMComponents(ISMComponents);

	for (UInstancedStaticMeshComponent* Component : ISMComponents)
	{
		if (!IsValid(Component))
		{
			continue;
		}
		Component->DestroyComponent();
		Component->MarkAsGarbage();
	}

	ReregisterAllComponents();

	MarkComponentsRenderStateDirty();
	MarkPackageDirty();
}

void ABuildingActor::ClearISMInstances()
{
	TArray<UInstancedStaticMeshComponent*> ISMComponents;
	GetISMComponents(ISMComponents);

	for (UInstancedStaticMeshComponent* Component : ISMComponents)
	{
		if (!IsValid(Component))
		{
			continue;
		}
		Component->ClearInstances();
	}
}








void ABuildingActor::ConstructBuilding()
{


	for (const FBuildingData& Data : BuildingData)
	{
		USplineComponent* Spline = FindComponentByTag<USplineComponent>(Data.SplineTag);
		if (!IsValid(Spline))
		{
			continue;
		}

	}

}
