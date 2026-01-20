// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "BuildingActor.h"

// Engine Headers
#include "Components/SplineComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/DynamicMeshComponent.h"
#include "UDynamicMesh.h"
#include "GeometryScript/MeshPrimitiveFunctions.h"

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



void ABuildingActor2::BuildDynamicMesh(UDynamicMeshComponent* DynamicMeshComponent)
{
	if (!IsValid(DynamicMeshComponent))
	{
		return;
	}

	UDynamicMesh* DynamicMesh = DynamicMeshComponent->GetDynamicMesh();
	if (!IsValid(DynamicMesh))
	{
		return;
	}

	FGeometryScriptPrimitiveOptions PrimitiveOptions;
	FTransform Transform;

	const TArray<FVector>& Vertices = BuildingParameters.Vertices;

	int Num = Vertices.Num();
	if (Num % 3 != 0)
	{
		return;
	}
	int TriCount = Num / 3;

	for (int i = 0; i < TriCount - 1; i++)
	{
		int Index = i * 3;
		TArray<FVector> Tri;
		Tri.Add(Vertices[Index]);
		Tri.Add(Vertices[Index + 1]);
		Tri.Add(Vertices[Index + 2]);
		UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendTriangulatedPolygon3D(DynamicMesh, PrimitiveOptions, Transform, Tri);
	}
}

