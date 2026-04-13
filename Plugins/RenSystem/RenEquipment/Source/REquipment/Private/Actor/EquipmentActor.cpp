// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Actor/EquipmentActor.h"

// Engine Headers
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Net/UnrealNetwork.h"

// Project Headers
#include "Definition/AscensionData.h"
#include "Interface/AscensionInstanceProvider.h"
#include "Interface/AssetInstanceCollection.h"
#include "Interface/AssetInstanceCollectionProvider.h"
#include "Library/AssetInstanceUtil.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Settings/EquipmentSettings.h"
#include "Asset/CoreDataAsset.h"
#include "Object/EquipmentMetadata.h"




AEquipmentActor::AEquipmentActor(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SetActorTickEnabled(false);
	SetActorHiddenInGame(true);
	SetCanBeDamaged(false);
}


void AEquipmentActor::BeginPlay()
{
	Super::BeginPlay();

	PostInitializeEquipment();
}

void AEquipmentActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	DeinitializeEquipment();

	Super::EndPlay(EndPlayReason);
}

AActor* AEquipmentActor::GetNextNode() const
{
	return _NextNode;
}

void AEquipmentActor::SetNextNode(AActor* Node)
{
	_NextNode = Node;
}





bool AEquipmentActor::GetIsEquipped() const
{
	return _bIsEquipped;
}

void AEquipmentActor::SetIsEquipped(bool IsEquipped)
{
	if (_bIsEquipped == IsEquipped)
	{
		PRINT_WARNING(LogTemp, 2.0f, TEXT("Already equipped or unequipped"));
		return;
	}

	_bIsEquipped = IsEquipped;

	if (IsEquipped)
	{
		OnEquipped();
	}
	else
	{
		OnUnequipped();
	}
}



void AEquipmentActor::InitializeEquipment()
{
	//IAbilitySystemInterface* OwnerAbility = GetOwner<IAbilitySystemInterface>();
	//if (OwnerAbility)
	//{
	//	OwnerASC = OwnerAbility->GetAbilitySystemComponent();

	//	const UEquipmentSettings* Settings = UEquipmentSettings::Get();
	//	if (IsValid(OwnerASC) && Settings->bAllowEventActivation && Settings->ActivateEventTags.HasTagExact(EquipmentData.EquipmentSlot))
	//	{
	//		const FEquipmentTagRelation* FoundRelation = Settings->EquipmentTagRelations.Find(EquipmentData.EquipmentSlot);
	//		if (FoundRelation && FoundRelation->EventTag.IsValid())
	//		{
	//			OwnerASC->GenericGameplayEventCallbacks.FindOrAdd(FoundRelation->EventTag).AddUObject(this, &AEquipmentActor::HandleActivationEvent);
	//		}
	//	}
	//}

	//if (EquipmentData.SourceType == EAssetQuerySource::Instance)
	//{
	//	InstanceCollection = FAssetInstanceUtil::GetPrimaryInstanceCollection(GetWorld(), EquipmentData.AssetId.PrimaryAssetType);
	//	if (InstanceCollection)
	//	{
	//		InstanceCollection->GetOnAssetInstanceCollectionUpdated().AddUObject(this, &AEquipmentActor::RefreshEquipment);
	//		InstanceAscension = Cast<IAscensionInstanceProvider>(InstanceCollection);
	//		if (InstanceAscension)
	//		{
	//			const FAscensionData* AscensionData = InstanceAscension->GetAscensionInstance(EquipmentData.AssetId, EquipmentData.EquipmentId);
	//			if (AscensionData)
	//			{
	//				SetEquipmentLevel(AscensionData->Level);
	//			}
	//		}
	//	}
	//}

	//PostInitializeEquipment();
}

void AEquipmentActor::PostInitializeEquipment()
{
	if (HasActorBegunPlay())
	{
		AttachEquipment();
		ApplyEffects();

		BP_InitializeEquipment();
	}
}

void AEquipmentActor::DeinitializeEquipment()
{
	DetachEquipment();
	RemoveEffects();
	SetEquipmentLevel(1);

	BP_DeinitializeEquipment();

	if (IsValid(OwnerASC))
	{
		FGameplayEventMulticastDelegate* FoundCallbacks = OwnerASC->GenericGameplayEventCallbacks.Find(EquipmentData.EquipmentSlot);
		if (FoundCallbacks)
		{
			FoundCallbacks->RemoveAll(this);
		}
	}
	OwnerASC = nullptr;

	if (InstanceCollection)
	{
		InstanceCollection->GetOnAssetInstanceCollectionUpdated().RemoveAll(this);
	}
	InstanceCollection = nullptr;
	InstanceAscension = nullptr;

	EquipmentAsset = nullptr;
	EquipmentOwnerId.Invalidate();
	EquipmentData.Reset();
}



void AEquipmentActor::HandleActivationEvent(const FGameplayEventData* Payload)
{
	if (!IsValid(OwnerASC) || !ActiveAbilityHandle.IsValid())
	{
		LOG_ERROR(LogTemp, TEXT("OwnerASC, AbilityHandle is invalid"));
		return;
	}

	if (Payload && Payload->EventTag == EquipmentData.EquipmentSlot)
	{
		const FGameplayAbilitySpec* AbilitySpec = OwnerASC->FindAbilitySpecFromHandle(ActiveAbilityHandle);
		if (AbilitySpec)
		{
			if (AbilitySpec->IsActive())
			{
				OwnerASC->CancelAbilityHandle(ActiveAbilityHandle);
			}
			else
			{
				OwnerASC->TryActivateAbility(ActiveAbilityHandle);
			}
		}
	}
}

void AEquipmentActor::ApplyEffects()
{
	if (!IsValid(OwnerASC) || !IsValid(EquipmentAsset))
	{
		LOG_ERROR(LogTemp, TEXT("OwnerASC, EquipmentAsset is invalid"));
		return;
	}

	//const UEquipmentMetadata* EquipmentMetadata = EquipmentAsset->FindFragmentByClass<UEquipmentMetadata>();
	//if (!IsValid(EquipmentMetadata))
	//{
	//	LOG_ERROR(LogTemp, TEXT("EquipmentMetadata is invalid"));
	//	return;
	//}
	//
	//const TArray<TSoftClassPtr<UGameplayEffect>>& EffectClasses = EquipmentMetadata->EffectClasses;
	//const TSoftClassPtr<UGameplayAbility>& AbilityClass = EquipmentMetadata->AbilityClass;

	//for (const TSoftClassPtr<UGameplayEffect>& SoftClass : EffectClasses)
	//{
	//	UClass* EffectClass = SoftClass.Get();
	//	if (!EffectClass)
	//	{
	//		continue;
	//	}

	//	FGameplayEffectContextHandle EffectContext = OwnerASC->MakeEffectContext();
	//	FGameplayEffectSpecHandle SpecHandle = OwnerASC->MakeOutgoingSpec(EffectClass, GetEquipmentLevel(), EffectContext);
	//	FActiveGameplayEffectHandle ActiveEffectHandle = OwnerASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

	//	ActiveEffectHandles.Add(ActiveEffectHandle);
	//}

	//if (AbilityClass.IsValid())
	//{
	//	FGameplayAbilitySpec AbilitySpec(AbilityClass.Get());
	//	AbilitySpec.Level = GetEquipmentLevel();
	//	AbilitySpec.DynamicAbilityTags.AddTag(EquipmentData.EquipmentSlot);
	//	AbilitySpec.SourceObject = this;

	//	ActiveAbilityHandle = OwnerASC->GiveAbility(AbilitySpec);
	//}
}

void AEquipmentActor::RemoveEffects()
{
	if (!IsValid(OwnerASC))
	{
		LOG_ERROR(LogTemp, TEXT("OwnerASC is invalid"));
		return;
	}

	for (const FActiveGameplayEffectHandle& Handle : ActiveEffectHandles)
	{
		OwnerASC->RemoveActiveGameplayEffect(Handle, 0);
	}
	ActiveEffectHandles.Empty();

	OwnerASC->ClearAbility(ActiveAbilityHandle);
}

void AEquipmentActor::RefreshEffects()
{
	if (!IsValid(OwnerASC))
	{
		LOG_ERROR(LogTemp, TEXT("OwnerASC is invalid"));
		return;
	}

	for (const FActiveGameplayEffectHandle& Handle : ActiveEffectHandles)
	{
		OwnerASC->SetActiveGameplayEffectLevel(Handle, GetEquipmentLevel());
	}

	FGameplayAbilitySpec* AbilitySpec = OwnerASC->FindAbilitySpecFromHandle(ActiveAbilityHandle);
	if (AbilitySpec)
	{
		AbilitySpec->Level = GetEquipmentLevel();
		OwnerASC->MarkAbilitySpecDirty(*AbilitySpec);
	}
}



void AEquipmentActor::RefreshEquipment()
{
	if (!InstanceAscension)
	{
		LOG_ERROR(LogTemp, TEXT("Equipment ascension instance is invalid"));
		return;
	}

	const FAscensionData* AscensionData = InstanceAscension->GetAscensionInstance(EquipmentData.AssetId, EquipmentData.EquipmentId);
	if (!AscensionData || GetEquipmentLevel() == AscensionData->Level)
	{
		LOG_WARNING(LogTemp, TEXT("Ascension data is invalid or level is not changed"));
		return;
	}

	SetEquipmentLevel(AscensionData->Level);
	RefreshEffects();
}



int AEquipmentActor::GetEquipmentLevel() const
{
	return _EquipmentLevel;
}

void AEquipmentActor::SetEquipmentLevel(int Level)
{
	_EquipmentLevel = FMath::Clamp(Level, 1, 1000);
}




void AEquipmentActor::OnEquipped()
{
	BP_OnEquipped();
}
void AEquipmentActor::OnUnequipped()
{
	BP_OnUnequipped();
}














void AEquipmentActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}



void AEquipmentActor::AttachEquipment_Implementation()
{
	AttachToActor(GetOwner(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void AEquipmentActor::DetachEquipment_Implementation()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

