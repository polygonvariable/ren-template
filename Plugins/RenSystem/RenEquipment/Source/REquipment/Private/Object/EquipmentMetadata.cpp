// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Object/EquipmentMetadata.h"

// Engine Headers
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"

// Project Headers
#include "Definition/AscensionData.h"
#include "Interface/AscensionInstanceProvider.h"
#include "Interface/AssetInstanceCollection.h"
#include "Library/AssetInstanceUtil.h"
#include "Interface/AssetInstanceCollectionProvider.h"
#include "Settings/EquipmentSettings.h"
#include "Actor/EquipmentActor.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Asset/CoreDataAsset.h"
#include "Asset/EquipmentDataAsset.h"


const UEquipmentDataDefinition* UEquipmentFragment::GetEquipmentDefinitionCDO() const
{
	if (!IsValid(EquipmentDefinition))
	{
		return nullptr;
	}
	return EquipmentDefinition->GetDefaultObject<UEquipmentDataDefinition>();
}

#if WITH_EDITORONLY_DATA
void UEquipmentFragment::AppendAssetBundleData(FAssetBundleData& Data)
{
	if (IsValid(EquipmentDefinition))
	{
		UEquipmentDataDefinition* DefinitionCDO = EquipmentDefinition->GetDefaultObject<UEquipmentDataDefinition>();
		if (IsValid(DefinitionCDO))
		{
			DefinitionCDO->AppendAssetBundleData(Data);
		}
	}
}

void UEquipmentDataDefinition::AppendAssetBundleData(FAssetBundleData& Data)
{
	const UEquipmentSettings* Settings = UEquipmentSettings::Get();
	const TArray<FName>& EquipmentBundles = Settings->EquipmentBundles;
	if (EquipmentBundles.IsValidIndex(0) && EquipmentBundles[0] != NAME_None)
	{
		const FName& BundleName = EquipmentBundles[0];

		Data.AddBundleAsset(BundleName, ControllerClass.ToSoftObjectPath().GetAssetPath());
		Data.AddBundleAsset(BundleName, ActorClass.ToSoftObjectPath().GetAssetPath());
		Data.AddBundleAsset(BundleName, AbilityCollection.ToSoftObjectPath().GetAssetPath());
	}
}
#endif




bool UEquipmentController::GetIsEquipped() const
{
	return false;
}

void UEquipmentController::SetIsEquipped(bool IsEquipped)
{
	if (_bIsEquipped == IsEquipped)
	{
		PRINT_WARNING(LogTemp, 2.0f, TEXT("Already equipped or unequipped"));
		return;
	}

	_bIsEquipped = IsEquipped;

	const UEquipmentSettings* Settings = UEquipmentSettings::Get();
	const FEquipmentTagRelation* FoundRelation = Settings->EquipmentTagRelations.Find(EquipmentData.EquipmentSlot);
	if (FoundRelation)
	{
		if (IsEquipped)
		{
			OwnerASC->AddLooseGameplayTag(FoundRelation->StateTag);
			OnEquipped();
		}
		else
		{
			OwnerASC->RemoveLooseGameplayTag(FoundRelation->StateTag);
			OnUnequipped();
		}
	}
}






int UEquipmentController::GetEquipmentLevel() const
{
	return _EquipmentLevel;
}

void UEquipmentController::SetEquipmentLevel(int Level)
{
	_EquipmentLevel = Level;
}

bool UEquipmentController::InitializeController(const UEquipmentDataDefinition* DataDefinition)
{
	if (!IsValid(EquipmentActor) || !IsValid(DataDefinition))
	{
		LOG_ERROR(LogTemp, TEXT("Equipment actor, data definition is invalid"));
		return false;
	}

	IAbilitySystemInterface* AbilityInterface = EquipmentActor->GetOwner<IAbilitySystemInterface>();
	if (AbilityInterface)
	{
		OwnerASC = AbilityInterface->GetAbilitySystemComponent();

		const UEquipmentSettings* Settings = UEquipmentSettings::Get();
		if (IsValid(OwnerASC) && Settings->bAllowEventActivation)
		{
			const FEquipmentTagRelation* FoundRelation = Settings->EquipmentTagRelations.Find(EquipmentData.EquipmentSlot);
			if (FoundRelation && FoundRelation->EventTag.IsValid())
			{
				OwnerASC->GenericGameplayEventCallbacks.FindOrAdd(FoundRelation->EventTag).AddUObject(this, &UEquipmentController::HandleActivationEvent);
			}
		}
	}

	if (EquipmentData.SourceType == EAssetQuerySource::Instance)
	{
		const FPrimaryAssetId& AssetId = EquipmentData.AssetId;

		InstanceCollection = FAssetInstanceUtil::GetPrimaryInstanceCollection(GetWorld(), AssetId.PrimaryAssetType);
		if (InstanceCollection)
		{
			InstanceCollection->GetOnAssetInstanceCollectionUpdated().AddUObject(this, &UEquipmentController::RefreshController);
			InstanceAscension = Cast<IAscensionInstanceProvider>(InstanceCollection);
			if (InstanceAscension)
			{
				const FAscensionData* AscensionData = InstanceAscension->GetAscensionInstance(AssetId, EquipmentData.EquipmentId);
				if (AscensionData)
				{
					SetEquipmentLevel(AscensionData->Level);
				}
			}
		}
	}

	_AbilityCollection = DataDefinition->AbilityCollection.Get();

	AttachEquipment();
	ApplyAbilities();

	BP_InitializeController();

	return true;
}

void UEquipmentController::DeinitializeController()
{
	BP_DeinitializeController();

	DetachEquipment();
	RemoveAbilities();

	const UEquipmentSettings* Settings = UEquipmentSettings::Get();
	const FEquipmentTagRelation* FoundRelation = Settings->EquipmentTagRelations.Find(EquipmentData.EquipmentSlot);
	if (IsValid(OwnerASC) && FoundRelation)
	{
		FGameplayEventMulticastDelegate* FoundCallbacks = OwnerASC->GenericGameplayEventCallbacks.Find(FoundRelation->EventTag);
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

	_EquipmentLevel = 1;
	_AbilityCollection = nullptr;
	_DataDefinition = nullptr;
}



void UEquipmentController::OnEquipped_Implementation()
{
	EquipmentActor->SetActorHiddenInGame(false);
}

void UEquipmentController::OnUnequipped_Implementation()
{
	EquipmentActor->SetActorHiddenInGame(true);
}



void UEquipmentController::RefreshController()
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
}

UWorld* UEquipmentController::GetWorld() const
{
	UObject* Outer = GetOuter();
	if (!IsValid(Outer))
	{
		return nullptr;
	}
	return Outer->GetWorld();
}

bool UEquipmentController::ImplementsGetWorld() const
{
	return true;
}






void UEquipmentController::ApplyAbilities()
{
	if (!IsValid(OwnerASC) || !IsValid(_AbilityCollection))
	{
		LOG_ERROR(LogTemp, TEXT("OwnerASC, ability collection is invalid"));
		return;
	}

	for (const TSubclassOf<UGameplayEffect>& EffectClass : _AbilityCollection->EffectClasses)
	{
		if (IsValid(EffectClass))
		{
			FGameplayEffectContextHandle EffectContext = OwnerASC->MakeEffectContext();
			FGameplayEffectSpecHandle SpecHandle = OwnerASC->MakeOutgoingSpec(EffectClass, GetEquipmentLevel(), EffectContext);
			FActiveGameplayEffectHandle ActiveEffectHandle = OwnerASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

			ActiveEffectHandles.Add(ActiveEffectHandle);
		}
	}

	const UEquipmentSettings* Settings = UEquipmentSettings::Get();
	const FEquipmentTagRelation* FoundRelation = Settings->EquipmentTagRelations.Find(EquipmentData.EquipmentSlot);

	for (const TSubclassOf<UGameplayAbility>& AbilityClass : _AbilityCollection->AbilityClasses)
	{
		if (IsValid(AbilityClass))
		{
			FGameplayAbilitySpec AbilitySpec(AbilityClass);
			AbilitySpec.Level = GetEquipmentLevel();
			AbilitySpec.DynamicAbilityTags.AddTag(FoundRelation->AbilityTag);
			AbilitySpec.SourceObject = this;

			ActiveAbilityHandles.Add(OwnerASC->GiveAbility(AbilitySpec));
		}
	}
}

void UEquipmentController::RemoveAbilities()
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

	for (const FGameplayAbilitySpecHandle& Handle : ActiveAbilityHandles)
	{
		OwnerASC->ClearAbility(Handle);
	}
	ActiveAbilityHandles.Empty();
}

void UEquipmentController::RefreshAbilities()
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

	for (const FGameplayAbilitySpecHandle& Handle : ActiveAbilityHandles)
	{
		FGameplayAbilitySpec* AbilitySpec = OwnerASC->FindAbilitySpecFromHandle(Handle);
		if (AbilitySpec)
		{
			AbilitySpec->Level = GetEquipmentLevel();
			OwnerASC->MarkAbilitySpecDirty(*AbilitySpec);
		}
	}
}





void UEquipmentController::HandleActivationEvent(const FGameplayEventData* Payload)
{
	if (!IsValid(OwnerASC) || !ActiveAbilityHandles.IsValidIndex(0) || !Payload)
	{
		LOG_ERROR(LogTemp, TEXT("OwnerASC, AbilityHandle, Payload is invalid"));
		return;
	}

	const UEquipmentSettings* Settings = UEquipmentSettings::Get();
	const FEquipmentTagRelation* FoundRelation = Settings->EquipmentTagRelations.Find(EquipmentData.EquipmentSlot);

	if (!FoundRelation || Payload->EventTag != FoundRelation->EventTag)
	{
		LOG_WARNING(LogTemp, TEXT("Tag relation is invalid or not found"));
		return;
	}

	if (OwnerASC->HasMatchingGameplayTag(FoundRelation->StateTag))
	{
		OwnerASC->CancelAbilityHandle(ActiveAbilityHandles[0]);
	}
	else
	{
		OwnerASC->TryActivateAbility(ActiveAbilityHandles[0]);
	}
}




void UEquipmentController::AttachEquipment_Implementation()
{
	if (!IsValid(EquipmentActor))
	{
		LOG_ERROR(LogTemp, TEXT("Equipment actor is invalid"));
		return;
	}
	
	AActor* Owner = EquipmentActor->GetOwner<AActor>();
	if (!IsValid(Owner))
	{
		LOG_ERROR(LogTemp, TEXT("Owner actor is invalid"));
		return;
	}

	EquipmentActor->AttachToActor(Owner, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void UEquipmentController::DetachEquipment_Implementation()
{
	if (!IsValid(EquipmentActor))
	{
		LOG_ERROR(LogTemp, TEXT("Equipment actor is invalid"));
		return;
	}

	EquipmentActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}


