// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// Project Headers

// Generated Headers
#include "BuildingActor.generated.h"

// Forward Declarations
class UInstancedStaticMeshComponent;
class UStaticMesh;
class USplineComponent;
class USceneComponent;



UENUM(BlueprintType)
enum EBuildingType : uint8
{
	Roof UMETA(DisplayName = "Roof"),
	Wall UMETA(DisplayName = "Wall")
};



USTRUCT(BlueprintType)
struct FBuildingData
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FName SplineTag = TEXT_EMPTY;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EBuildingType> BuildingType;

	UPROPERTY(EditAnywhere)
	bool bFill = false;

	UPROPERTY(EditAnywhere)
	int Height = 1;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMesh> StaticMesh = nullptr;

	UPROPERTY(EditAnywhere)
	float SegmentSize = 100.0f;

};




/**
 * 
 * TODO:
 * Replace this with PCG elements, in which those elements will create points.
 * which can be used to spawn things like wall, fence, etc.
 * 
 */
UCLASS()
class ABuildingActor : public AActor
{

	GENERATED_BODY()

public:

	ABuildingActor();


	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<USceneComponent> SceneComponent;

	UPROPERTY(EditAnywhere)
	TArray<FBuildingData> BuildingData;



	void GetAllStaticMeshes(TSet<UStaticMesh*>& OutMeshes) const;

	UFUNCTION(BlueprintCallable, CallInEditor)
	void MakeISMComponents();

	void GetISMComponents(TArray<UInstancedStaticMeshComponent*>& OutISMComponents) const;

	UFUNCTION(BlueprintCallable, CallInEditor)
	void DestroyISMComponents();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void ClearISMInstances();



	UFUNCTION(BlueprintCallable)
	void ConstructBuilding();

};

