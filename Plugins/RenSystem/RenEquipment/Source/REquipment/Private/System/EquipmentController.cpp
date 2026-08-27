// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/EquipmentController.h"

// Engine Headers
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"

// Project Headers
#include "Actor/EquipmentActor.h"
#include "Core/AssetInstanceUtil.h"
#include "Core/EquipmentSettings.h"
#include "Core/Interface/AscensionInstanceProvider.h"
#include "Core/Interface/AssetInstanceCollection.h"
#include "Core/Type/AscensionData.h"
#include "Data/CoreDataAsset.h"
#include "Data/EquipmentAbilityCollection.h"
#include "Data/EquipmentDataDefinition.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"


bool UEquipmentController::InitializeController(const UCoreDataAsset* InEquipmentAsset, const FEquipmentInitializationData& InEquipmentData, AEquipmentActor* InEquipmentActor, const UEquipmentDataDefinition* InDataDefinition)
{
	if (IsInitialized())
	{
		LOG_ERROR(LogEquipment, TEXT("Controller is already initialized"));
		return false;
	}

	if (!IsValid(InEquipmentAsset) || !IsValid(InEquipmentActor) || !IsValid(InDataDefinition) || !InEquipmentData.IsValid())
	{
		LOG_ERROR(LogEquipment, TEXT("Equipment actor, data definition, equipment data is invalid"));
		return false;
	}

	EquipmentAsset = InEquipmentAsset;
	EquipmentData = InEquipmentData;
	EquipmentActor = InEquipmentActor;
	DataDefinition = InDataDefinition;

	AttachEquipment();

	InitializeGameplayEvent();
	InitializeAssetInstance();
	CreateAbilities();

	SetIsInitialized(true);
	OnControllerInitialized();

	return true;
}

void UEquipmentController::DeinitializeController()
{
	if (!IsInitialized())
	{
		return;
	}

	DeactivateEquipment(true);
	DetachEquipment();

	DeinitializeGameplayEvent();
	DeinitializeAssetInstance();
	RemoveAbilities();

	EquipmentAsset = nullptr;
	EquipmentData.Reset();
	EquipmentActor = nullptr;

	DataDefinition = nullptr;

	_OwnerAbilitySystem = nullptr;
	_OwnerAnimInstance = nullptr;

	SetIsInitialized(false);
	OnControllerDeinitialized();
}


bool UEquipmentController::ActivateEquipment()
{
	return false;
}

bool UEquipmentController::DeactivateEquipment(bool bForce)
{
	return false;
}

void UEquipmentController::RefreshEquipment()
{
	if (!InstanceAscension)
	{
		LOG_ERROR(LogEquipment, TEXT("Equipment ascension instance is invalid"));
		return;
	}

	const FAscensionData* AscensionData = InstanceAscension->GetAscensionInstance(EquipmentData.AssetId, EquipmentData.AssetInstanceId);
	if (!AscensionData || GetEquipmentLevel() == AscensionData->Level)
	{
		LOG_WARNING(LogEquipment, TEXT("Ascension data is invalid or level is not changed"));
		return;
	}

	SetEquipmentLevel(AscensionData->Level);
}

const UCoreDataAsset* UEquipmentController::GetEquipmentAsset() const
{
	return EquipmentAsset;
}

const FEquipmentInitializationData& UEquipmentController::GetEquipmentData() const
{
	return EquipmentData;
}


const UEquipmentAbilityCollection* UEquipmentController::GetEquipmentAbilityCollection() const
{
	return DataDefinition->AbilityCollection.Get();
}

const UEquipmentDataDefinition* UEquipmentController::GetEquipmentDataDefinition() const
{
	return DataDefinition;
}

AEquipmentActor* UEquipmentController::GetEquipmentActor() const
{
	return EquipmentActor;
}

int UEquipmentController::GetEquipmentLevel() const
{
	return _Level;
}


bool UEquipmentController::IsInitialized() const
{
	return _bIsInitialized;
}

bool UEquipmentController::IsAttached() const
{
	return _bIsAttached;
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

#if WITH_EDITOR
bool UEquipmentController::ImplementsGetWorld() const
{
	return true;
}
#endif


void UEquipmentController::InitializeAssetInstance()
{
	if (SourceType == EDataSource::Runtime)
	{
		const FPrimaryAssetId& AssetId = EquipmentData.AssetId;

		InstanceCollection = FAssetInstanceUtil::GetPrimaryInstanceCollection(GetWorld(), AssetId.PrimaryAssetType);
		if (InstanceCollection)
		{
			InstanceCollection->GetOnAssetInstanceCollectionUpdated().AddUObject(this, &UEquipmentController::RefreshEquipment);
			InstanceAscension = Cast<IAscensionInstanceProvider>(InstanceCollection);
			if (InstanceAscension)
			{
				const FAscensionData* AscensionData = InstanceAscension->GetAscensionInstance(AssetId, EquipmentData.AssetInstanceId);
				if (AscensionData)
				{
					SetEquipmentLevel(AscensionData->Level);
				}
			}
		}
	}
}

void UEquipmentController::DeinitializeAssetInstance()
{
	if (InstanceCollection)
	{
		InstanceCollection->GetOnAssetInstanceCollectionUpdated().RemoveAll(this);
	}
	InstanceCollection = nullptr;
	InstanceAscension = nullptr;
	_Level = 1;
}

void UEquipmentController::InitializeGameplayEvent()
{

}

void UEquipmentController::DeinitializeGameplayEvent()
{

}


bool UEquipmentController::CanActivate() const
{
	UAbilitySystemComponent* AbilitySystem = GetOwnerAbilitySystemComponent();
	if (AbilitySystem->HasAnyMatchingGameplayTags(DataDefinition->DeactivationTag))
	{
		return false;
	}
	return true;
}


void UEquipmentController::CreateAbilities()
{
	const FEquipmentSlotData* SlotData = UEquipmentSettings::GetEquipmentSlotById(EquipmentData.SlotId);
	const UEquipmentAbilityCollection* AbilityCollection = GetEquipmentAbilityCollection();
	UAbilitySystemComponent* AbilitySystem = GetOwnerAbilitySystemComponent();
	if (!SlotData || !IsValid(AbilitySystem) || !IsValid(AbilityCollection))
	{
		LOG_ERROR(LogEquipment, TEXT("SlotData, AbilitySystem, AbilityCollection is invalid"));
		return;
	}

	for (const TSubclassOf<UGameplayEffect>& EffectClass : AbilityCollection->EffectClasses)
	{
		if (IsValid(EffectClass))
		{
			FGameplayEffectContextHandle EffectContext = AbilitySystem->MakeEffectContext();
			FGameplayEffectSpecHandle SpecHandle = AbilitySystem->MakeOutgoingSpec(EffectClass, GetEquipmentLevel(), EffectContext);
			FActiveGameplayEffectHandle ActiveEffectHandle = AbilitySystem->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

			ActiveEffectHandles.Add(ActiveEffectHandle);
		}
	}

	const TArray<TSubclassOf<UGameplayAbility>>& Abilities = AbilityCollection->Abilities;
	for (const TSubclassOf<UGameplayAbility>& AbilityClass : Abilities)
	{
		if (IsValid(AbilityClass))
		{
			FGameplayAbilitySpec AbilitySpec(AbilityClass);
			AbilitySpec.Level = GetEquipmentLevel();
			AbilitySpec.SourceObject = this;
			AbilitySpec.InputID = SlotData->InputId;

			FGameplayAbilitySpecHandle AbilityHandle = AbilitySystem->GiveAbility(AbilitySpec);
			ActiveAbilityHandles.Add(AbilityHandle);
		}
	}
}

void UEquipmentController::RemoveAbilities()
{
	UAbilitySystemComponent* AbilitySystem = GetOwnerAbilitySystemComponent();
	if (!IsValid(AbilitySystem))
	{
		LOG_ERROR(LogEquipment, TEXT("AbilitySystem is invalid"));
		return;
	}

	for (const FActiveGameplayEffectHandle& Handle : ActiveEffectHandles)
	{
		AbilitySystem->RemoveActiveGameplayEffect(Handle, 0);
	}
	ActiveEffectHandles.Empty();

	for (const FGameplayAbilitySpecHandle& Handle : ActiveAbilityHandles)
	{
		AbilitySystem->ClearAbility(Handle);
	}
	ActiveAbilityHandles.Empty();
}

void UEquipmentController::RefreshAbilities()
{
	UAbilitySystemComponent* AbilitySystem = GetOwnerAbilitySystemComponent();
	if (!IsValid(AbilitySystem))
	{
		LOG_ERROR(LogEquipment, TEXT("AbilitySystem is invalid"));
		return;
	}

	for (const FActiveGameplayEffectHandle& Handle : ActiveEffectHandles)
	{
		AbilitySystem->SetActiveGameplayEffectLevel(Handle, GetEquipmentLevel());
	}

	for (const FGameplayAbilitySpecHandle& Handle : ActiveAbilityHandles)
	{
		FGameplayAbilitySpec* AbilitySpec = AbilitySystem->FindAbilitySpecFromHandle(Handle);
		if (AbilitySpec)
		{
			AbilitySpec->Level = GetEquipmentLevel();
			AbilitySystem->MarkAbilitySpecDirty(*AbilitySpec);
		}
	}
}


void UEquipmentController::AttachEquipment()
{
	if (!IsValid(EquipmentActor) || IsAttached())
	{
		LOG_ERROR(LogEquipment, TEXT("Equipment actor is invalid or already attached"));
		return;
	}

	EquipmentActor->AttachToActor(GetEquipmentOwner(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	SetIsAttached(true);
}

void UEquipmentController::DetachEquipment()
{
	if (!IsValid(EquipmentActor) || !IsAttached())
	{
		LOG_ERROR(LogEquipment, TEXT("Equipment actor is invalid or not attached"));
		return;
	}

	EquipmentActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SetIsAttached(false);
}


void UEquipmentController::OnControllerInitialized()
{
}

void UEquipmentController::OnControllerDeinitialized()
{
}


UAnimInstance* UEquipmentController::GetOwnerAnimInstance() const
{
	if (!IsValid(_OwnerAnimInstance))
	{
		ACharacter* Character = GetEquipmentOwner<ACharacter>();
		if (!IsValid(Character))
		{
			return nullptr;
		}

		USkeletalMeshComponent* Mesh = Character->GetMesh();
		if (!IsValid(Mesh))
		{
			return nullptr;
		}

		_OwnerAnimInstance = Mesh->GetAnimInstance();
	}
	return _OwnerAnimInstance;
}

UAbilitySystemComponent* UEquipmentController::GetOwnerAbilitySystemComponent() const
{
	if (!IsValid(_OwnerAbilitySystem))
	{
		IAbilitySystemInterface* AbilityInterface = GetEquipmentOwner<IAbilitySystemInterface>();
		if (AbilityInterface)
		{
			_OwnerAbilitySystem = AbilityInterface->GetAbilitySystemComponent();
		}
	}
	return _OwnerAbilitySystem;
}

AActor* UEquipmentController::GetEquipmentOwner() const
{
	if (!IsValid(EquipmentActor))
	{
		return nullptr;
	}
	return EquipmentActor->GetOwner();
}


void UEquipmentController::SetEquipmentLevel(int Level)
{
	_Level = Level;
}

void UEquipmentController::SetIsAttached(bool bAttached)
{
	_bIsAttached = bAttached;
}

void UEquipmentController::SetIsInitialized(bool bInitialized)
{
	_bIsInitialized = bInitialized;
}

