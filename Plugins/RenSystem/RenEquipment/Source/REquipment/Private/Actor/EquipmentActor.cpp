// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Actor/EquipmentActor.h"

// Project Headers
#include "Interface/AscensionInstanceProvider.h"
#include "Interface/AssetInstanceCollection.h"
#include "Interface/AssetInstanceCollectionProvider.h"
#include "Library/AssetInstanceUtil.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"


AActor* AEquipmentActor::GetNextNode() const
{
	return _NextNode;
}

void AEquipmentActor::SetNextNode(AActor* Node)
{
	_NextNode = Cast<AEquipmentActor>(Node);
}

void AEquipmentActor::InitializeEquipment()
{
	AttachToActor(GetOwner(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	IAssetInstanceCollectionProvider* CollectionProvider = FAssetInstanceUtil::GetInstanceCollectionProvider(GetWorld(), SpawnData.AssetId);
	if (CollectionProvider)
	{
		FName CollectionId = CollectionProvider->GetDefaultCollectionId();
		InstanceCollection = CollectionProvider->GetInstanceCollection(CollectionId);
		if (InstanceCollection)
		{
			InstanceCollection->GetOnAssetInstanceCollectionUpdated().AddUObject(this, &AEquipmentActor::RefreshEquipment);

			IAscensionInstanceProvider* AscensionInterface = Cast<IAscensionInstanceProvider>(InstanceCollection);
			if (AscensionInterface)
			{
				const FAscensionData* AscensionData = AscensionInterface->GetAscensionInstance(SpawnData.AssetId, SpawnData.EquipmentId);
				if (AscensionData)
				{
					SpawnData.AscensionData = *AscensionData;
				}
			}
		}
	}

	BP_InitializeEquipment();
}

void AEquipmentActor::DeinitializeEquipment()
{
	BP_DeinitializeEquipment();

	if (InstanceCollection)
	{
		InstanceCollection->GetOnAssetInstanceCollectionUpdated().RemoveAll(this);
	}

	InstanceCollection = nullptr;
	EquipmentAsset = nullptr;
	OwnerId.Invalidate();
	SpawnData.Reset();

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

void AEquipmentActor::RefreshEquipment()
{
	IAscensionInstanceProvider* AscensionInterface = Cast<IAscensionInstanceProvider>(InstanceCollection);
	if (!AscensionInterface)
	{
		return;
	}

	const FAscensionData* AscensionData = AscensionInterface->GetAscensionInstance(SpawnData.AssetId, SpawnData.EquipmentId);
	if (!AscensionData)
	{
		return;
	}

	SpawnData.AscensionData = *AscensionData;
}

void AEquipmentActor::BeginPlay()
{
	Super::BeginPlay();
}

void AEquipmentActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	DeinitializeEquipment();

	Super::EndPlay(EndPlayReason);
}



void AEquipmentActor::BP_InitializeEquipment_Implementation()
{
}

void AEquipmentActor::BP_DeinitializeEquipment_Implementation()
{
}
