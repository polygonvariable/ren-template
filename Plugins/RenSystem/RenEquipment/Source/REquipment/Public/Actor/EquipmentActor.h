// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameFramework/Actor.h"

// Project Headers
#include "Definition/EquipmentSpawnData.h"
#include "Interface/IActorLinkedNode.h"

// Generated Headers
#include "EquipmentActor.generated.h"

// Forward Declarations
class UCoreDataAsset;
class IAssetInstanceCollection;


/**
 *
 *
 */
UCLASS(Abstract)
class AEquipmentActor : public AActor, public IActorLinkedNode
{

	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<const UCoreDataAsset> EquipmentAsset;

	UPROPERTY(VisibleAnywhere)
	FGuid OwnerId;

	UPROPERTY(VisibleAnywhere)
	FEquipmentData EquipmentData;


	void InitializeEquipment();
	void DeinitializeEquipment();

	void RefreshEquipment();

	// ~ IActorLinkedNode
	virtual AActor* GetNextNode() const override final;
	virtual void SetNextNode(AActor* Node) override final;
	// ~ End of IActorLinkedNode

	// ~ AActor
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// ~ End of AActor

protected:

	IAssetInstanceCollection* InstanceCollection;


	UFUNCTION(BlueprintNativeEvent, Meta = (DisplayName = "Initialize Equipment"))
	void BP_InitializeEquipment();
	virtual void BP_InitializeEquipment_Implementation();

	UFUNCTION(BlueprintNativeEvent, Meta = (DisplayName = "Deinitialize Equipment"))
	void BP_DeinitializeEquipment();
	virtual void BP_DeinitializeEquipment_Implementation();

private:

	UPROPERTY()
	AEquipmentActor* _NextNode = nullptr;

};

