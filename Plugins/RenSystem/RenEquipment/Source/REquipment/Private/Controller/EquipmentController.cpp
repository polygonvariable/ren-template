// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Controller/EquipmentController.h"

// Engine Headers
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"

// Project Headers
#include "Actor/EquipmentActor.h"
#include "Asset/CoreDataAsset.h"
#include "Asset/EquipmentDataAsset.h"
#include "Definition/AscensionData.h"
#include "Interface/AscensionInstanceProvider.h"
#include "Interface/AssetInstanceCollection.h"
#include "Interface/AssetInstanceCollectionProvider.h"
#include "Library/AssetInstanceUtil.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Object/EquipmentMetadata.h"
#include "Settings/EquipmentSettings.h"




void UEquipmentController::ActivateEquipment()
{
}
void UEquipmentController::DeactivateEquipment()
{
}


int UEquipmentController::GetEquipmentLevel() const
{
	return _Level;
}

void UEquipmentController::SetEquipmentLevel(int Level)
{
	_Level = Level;
}


bool UEquipmentController::InitializeController(const UEquipmentDataDefinition* InDataDefinition)
{
	EquipmentTagData = UEquipmentSettings::GetTagData(EquipmentData.EquipmentSlot);
	if (!IsValid(EquipmentActor) || !IsValid(InDataDefinition) || !EquipmentData.IsValid() || !EquipmentTagData)
	{
		LOG_ERROR(LogTemp, TEXT("Equipment actor, data definition, equipment data, tag data is invalid"));
		return false;
	}

	const UEquipmentSettings* Settings = UEquipmentSettings::Get();
	UAbilitySystemComponent* AbilitySystem = GetOwnerAbilitySystemComponent();
	if (IsValid(AbilitySystem) && Settings->bAllowEventActivation)
	{
		if (EquipmentTagData->EventTag.IsValid())
		{
			AbilitySystem->GenericGameplayEventCallbacks.FindOrAdd(EquipmentTagData->EventTag).AddUObject(this, &UEquipmentController::HandleActivationEvent);
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

	DataDefinition = InDataDefinition;
	AbilityCollection = InDataDefinition->AbilityCollection.Get();

	SetIsDeinitialized(false);

	AttachEquipment();
	ApplyAbilities();

	OnControllerInitialized();
	return true;
}

void UEquipmentController::DeinitializeController()
{
	SetIsDeinitialized(true);

	OnControllerDeinitialized();

	DetachEquipment();
	RemoveAbilities();

	UAbilitySystemComponent* AbilitySystem = GetOwnerAbilitySystemComponent();
	if (IsValid(AbilitySystem))
	{
		const UEquipmentSettings* Settings = UEquipmentSettings::Get();
		if (EquipmentTagData)
		{
			FGameplayEventMulticastDelegate* Callback = AbilitySystem->GenericGameplayEventCallbacks.Find(EquipmentTagData->EventTag);
			if (Callback)
			{
				Callback->RemoveAll(this);
			}
		}

		FGameplayEventMulticastDelegate* EquipCallback = AbilitySystem->GenericGameplayEventCallbacks.Find(Settings->EquipmentEquipNotify);
		if (EquipCallback)
		{
			EquipCallback->RemoveAll(this);
		}
		
		FGameplayEventMulticastDelegate* UnequipCallback = AbilitySystem->GenericGameplayEventCallbacks.Find(Settings->EquipmentUnequipNotify);
		if (UnequipCallback)
		{
			UnequipCallback->RemoveAll(this);
		}
	}

	UAnimInstance* AnimInstance = GetOwnerAnimInstance();
	if (IsValid(AnimInstance))
	{
		AnimInstance->UnlinkAnimClassLayers(EquipmentAnimInstance);
		AnimInstance->Montage_Stop(0.0f, EquipAnimation);
		AnimInstance->Montage_Stop(0.0f, UnequipAnimation);
	}

	_OwnerAbilitySystem = nullptr;
	_OwnerAnimInstance = nullptr;

	if (InstanceCollection)
	{
		InstanceCollection->GetOnAssetInstanceCollectionUpdated().RemoveAll(this);
	}
	InstanceCollection = nullptr;
	InstanceAscension = nullptr;

	EquipmentTagData = nullptr;
	AbilityCollection = nullptr;
	DataDefinition = nullptr;

	EquipmentAsset = nullptr;
	EquipmentData.Reset();
	EquipmentActor = nullptr;

	_Level = 1;
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

void UEquipmentController::OnEquipped()
{
}

void UEquipmentController::OnUnequipped()
{
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

AActor* UEquipmentController::GetEquipmentOwner() const
{
	if (!IsValid(EquipmentActor))
	{
		return nullptr;
	}
	return EquipmentActor->GetOwner();
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

AActor* UEquipmentController::GetEquipmentActor() const
{
	return EquipmentActor;
}

void UEquipmentController::HandleAbilityActivation(const FGameplayEventData* Payload, FGameplayTag EventTag)
{
	UAbilitySystemComponent* AbilitySystem = GetOwnerAbilitySystemComponent();
	if (IsValid(AbilitySystem))
	{
		AbilitySystem->TryActivateAbilitiesByTag(FGameplayTagContainer(EventTag));
	}
}

void UEquipmentController::ApplyAbilities()
{
	UAbilitySystemComponent* AbilitySystem = GetOwnerAbilitySystemComponent();
	if (!IsValid(AbilitySystem) || !IsValid(AbilityCollection) || !EquipmentTagData)
	{
		LOG_ERROR(LogTemp, TEXT("AbilitySystem, ability collection, equipment tag data is invalid"));
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

	//const TArray<FEquipmentAbilityData>& Abilities = AbilityCollection->Abilities;
	//int AbilityNum = Abilities.Num();
	//for (int i = 0; i < AbilityNum; i++)
	//{
	//	const FGameplayTag& EventTag = Abilities[i].EventTag;
	//	const TArray<TSubclassOf<UGameplayAbility>>& AbilityClasses = Abilities[i].AbilityClasses;

	//	for (const TSubclassOf<UGameplayAbility>& AbilityClass : AbilityClasses)
	//	{
	//		if (IsValid(AbilityClass))
	//		{
	//			FGameplayAbilitySpec AbilitySpec(AbilityClass);
	//			AbilitySpec.Level = GetEquipmentLevel();
	//			AbilitySpec.SourceObject = this;
	//			if (i == 0)
	//			{
	//				AbilitySpec.DynamicAbilityTags.AddTag(EquipmentTagData->AbilityTag);
	//			}
	//			else
	//			{
	//				AbilitySpec.DynamicAbilityTags.AddTag(EquipmentTagData->StateTag);
	//			}

	//			FGameplayAbilitySpecHandle AbilityHandle = AbilitySystem->GiveAbility(AbilitySpec);
	//			ActiveAbilityHandles.Add(AbilityHandle);
	//		}
	//	}

	//	if (EventTag.IsValid() && AbilityClasses.Num() > 0)
	//	{
	//		AbilitySystem->GenericGameplayEventCallbacks.FindOrAdd(EventTag).AddUObject(this, &UEquipmentController::HandleAbilityActivation, EventTag);
	//	}
	//}
	
	const TArray<TSubclassOf<UGameplayAbility>>& Abilities = AbilityCollection->AbilityClasses;
	int AbilityNum = Abilities.Num();

	for (int i = 0; i < AbilityNum; i++)
	{
		const TSubclassOf<UGameplayAbility>& AbilityClass = Abilities[i];
		if (IsValid(AbilityClass))
		{
			FGameplayAbilitySpec AbilitySpec(AbilityClass);
			AbilitySpec.Level = GetEquipmentLevel();
			AbilitySpec.SourceObject = this;
			if (i == 0)
			{
				AbilitySpec.DynamicAbilityTags.AddTag(EquipmentTagData->AbilityTag);
			}
			else
			{
				AbilitySpec.DynamicAbilityTags.AddTag(EquipmentTagData->StateTag);
			}

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
		LOG_ERROR(LogTemp, TEXT("AbilitySystem is invalid"));
		return;
	}

	//for (const FEquipmentAbilityData& Data : AbilityCollection->Abilities)
	//{
	//	const FGameplayTag& EventTag = Data.EventTag;
	//	if (EventTag.IsValid())
	//	{
	//		AbilitySystem->GenericGameplayEventCallbacks.FindOrAdd(EventTag).RemoveAll(this);
	//	}
	//}

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
		LOG_ERROR(LogTemp, TEXT("AbilitySystem is invalid"));
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

void UEquipmentController::HandleActivationEvent(const FGameplayEventData* Payload)
{
}

void UEquipmentController::AttachEquipment()
{
	if (!IsValid(EquipmentActor))
	{
		LOG_ERROR(LogTemp, TEXT("Equipment actor is invalid"));
		return;
	}

	EquipmentActor->AttachToActor(GetEquipmentOwner(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void UEquipmentController::DetachEquipment()
{
	if (!IsValid(EquipmentActor))
	{
		LOG_ERROR(LogTemp, TEXT("Equipment actor is invalid"));
		return;
	}

	EquipmentActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}


bool UEquipmentController::GetIsDeinitialized() const
{
	return _bIsDeinitialized;
}

void UEquipmentController::SetIsDeinitialized(bool bState)
{
	_bIsDeinitialized = bState;
}













void UEquipmentController_Weapon::OnControllerDeinitialized_Implementation()
{
	UnlinkAnimInstance();
	OnUnequipped();
}


void UEquipmentController_Weapon::HandleActivationEvent(const FGameplayEventData* Payload)
{
	UAbilitySystemComponent* AbilitySystem = GetOwnerAbilitySystemComponent();
	if (!IsValid(AbilitySystem) || !ActiveAbilityHandles.IsValidIndex(0) || !Payload)
	{
		LOG_ERROR(LogTemp, TEXT("AbilitySystem, AbilityHandle, Payload is invalid"));
		return;
	}

	if (!EquipmentTagData || Payload->EventTag != EquipmentTagData->EventTag)
	{
		LOG_WARNING(LogTemp, TEXT("Tag data is invalid or not found"));
		return;
	}

	if (AbilitySystem->HasMatchingGameplayTag(EquipmentTagData->StateTag))
	{
		AbilitySystem->CancelAbilityHandle(ActiveAbilityHandles[0]);
	}
	else
	{
		AbilitySystem->TryActivateAbility(ActiveAbilityHandles[0]);
	}
}

void UEquipmentController_Weapon::ActivateEquipment()
{
	if (IsEquipped() || GetIsDeinitialized())
	{
		LOG_WARNING(LogTemp, TEXT("Equipment is already equipped or is deinitialized"));
		return;
	}

	LinkAnimInstance();
	if (!PlayEquipAnimation())
	{
		AttachToEquipSocket();
		OnEquipped();
	}
}

void UEquipmentController_Weapon::DeactivateEquipment()
{
	if (!IsEquipped() || GetIsDeinitialized())
	{
		LOG_WARNING(LogTemp, TEXT("Equipment is already unequipped or is deinitialized"));
		return;
	}

	if (!PlayUnequipAnimation())
	{
		UnlinkAnimInstance();
		AttachToUnequipSocket();
		OnUnequipped();
	}
}

bool UEquipmentController_Weapon::IsEquipped()
{
	UAbilitySystemComponent* AbilitySystem = GetOwnerAbilitySystemComponent();
	if (!EquipmentTagData || !IsValid(AbilitySystem))
	{
		return false;
	}
	return AbilitySystem->HasMatchingGameplayTag(EquipmentTagData->StateTag);
}


void UEquipmentController_Weapon::OnEquipped()
{
	UAbilitySystemComponent* AbilitySystem = GetOwnerAbilitySystemComponent();
	if (EquipmentTagData && IsValid(AbilitySystem))
	{
		AbilitySystem->AddLooseGameplayTag(EquipmentTagData->StateTag);
	}
}

void UEquipmentController_Weapon::OnUnequipped()
{
	UAbilitySystemComponent* AbilitySystem = GetOwnerAbilitySystemComponent();
	if (EquipmentTagData && IsValid(AbilitySystem))
	{
		AbilitySystem->RemoveLooseGameplayTag(EquipmentTagData->StateTag);
	}
}


bool UEquipmentController_Weapon::LinkAnimInstance()
{
	UAnimInstance* AnimInstance = GetOwnerAnimInstance();
	if (!IsValid(AnimInstance) || !(IsValid(EquipmentAnimInstance)))
	{
		return false;
	}
	AnimInstance->LinkAnimClassLayers(EquipmentAnimInstance);
	return true;
}

bool UEquipmentController_Weapon::UnlinkAnimInstance()
{
	UAnimInstance* AnimInstance = GetOwnerAnimInstance();
	if (!IsValid(AnimInstance) || !(IsValid(EquipmentAnimInstance)))
	{
		return false;
	}
	AnimInstance->UnlinkAnimClassLayers(EquipmentAnimInstance);
	return true;
}

bool UEquipmentController_Weapon::PlayEquipAnimation()
{
	UAnimInstance* AnimInstance = GetOwnerAnimInstance();
	if (!IsValid(AnimInstance) || !IsValid(EquipAnimation))
	{
		LOG_ERROR(LogTemp, TEXT("Anim instance, equip animation is invalid"));
		return false;
	}

	float PlayDuration = AnimInstance->Montage_Play(EquipAnimation, 1.0f);
	if (PlayDuration <= 0.0f)
	{
		LOG_ERROR(LogTemp, TEXT("Failed to play equip animation"));
		return false;
	}

	RegisterAttachmentNotify(true);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &UEquipmentController_Weapon::HandleMontageEquipEnded);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, EquipAnimation);

	return true;
}

bool UEquipmentController_Weapon::PlayUnequipAnimation()
{
	UAnimInstance* AnimInstance = GetOwnerAnimInstance();
	if (!IsValid(AnimInstance) || !IsValid(UnequipAnimation))
	{
		LOG_ERROR(LogTemp, TEXT("Anim instance, unequip animation is invalid"));
		return false;
	}

	float PlayDuration = AnimInstance->Montage_Play(UnequipAnimation, 1.0f);
	if (PlayDuration <= 0.0f)
	{
		LOG_ERROR(LogTemp, TEXT("Failed to play unequip animation"));
		return false;
	}

	RegisterAttachmentNotify(false);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &UEquipmentController_Weapon::HandleMontageUnequipEnded);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, UnequipAnimation);

	return true;
}

void UEquipmentController_Weapon::HandleMontageAttachmentNotify(const FGameplayEventData* Payload, bool bIsEquip)
{
	UnregisterAttachmentNotify(bIsEquip);

	if (bIsEquip)
	{
		AttachToEquipSocket();
	}
	else
	{
		AttachToUnequipSocket();
	}
}

void UEquipmentController_Weapon::AttachToEquipSocket()
{
	ACharacter* Character = GetEquipmentOwner<ACharacter>();
	if (IsValid(Character))
	{
		FTransform SocketTransform = FTransform::Identity;
		FName SocketName = NAME_None;

		const UEquipmentDataDefinition_Weapon* WeaponDefinition = Cast<UEquipmentDataDefinition_Weapon>(DataDefinition);
		if (IsValid(WeaponDefinition))
		{
			SocketName = WeaponDefinition->EquipSocket.SocketName;
			SocketTransform = WeaponDefinition->EquipSocket.SocketTransform;
		}

		EquipmentActor->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
		EquipmentActor->SetActorRelativeTransform(SocketTransform);
	}

	EquipmentState = EEquipmentState::Equipped;
}

void UEquipmentController_Weapon::AttachToUnequipSocket()
{
	ACharacter* Character = GetEquipmentOwner<ACharacter>();
	if (IsValid(Character))
	{
		FTransform SocketTransform = FTransform::Identity;
		FName SocketName = NAME_None;

		const UEquipmentDataDefinition_Weapon* WeaponDefinition = Cast<UEquipmentDataDefinition_Weapon>(DataDefinition);
		if (IsValid(WeaponDefinition))
		{
			SocketName = WeaponDefinition->UnequipSocket.SocketName;
			SocketTransform = WeaponDefinition->UnequipSocket.SocketTransform;
		}

		if (EquipmentTagData)
		{
			USpringArmComponent* TargetComponent = Character->FindComponentByTag<USpringArmComponent>(EquipmentTagData->ComponentName);
			if (IsValid(TargetComponent))
			{
				EquipmentActor->AttachToComponent(TargetComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
				EquipmentActor->SetActorRelativeTransform(SocketTransform);
			}
		}
	}

	EquipmentState = EEquipmentState::Unequipped;
}

void UEquipmentController_Weapon::HandleMontageEquipEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// if unable to equip via montage then force it when montage ends
	if (EquipmentState == EEquipmentState::Unequipped)
	{
		UnregisterAttachmentNotify(true);
		AttachToEquipSocket();
	}

	if (!IsEquipped())
	{
		OnEquipped();
	}
}

void UEquipmentController_Weapon::HandleMontageUnequipEnded(UAnimMontage* Montage, bool bInterrupted)
{
	UnlinkAnimInstance();

	if (EquipmentState == EEquipmentState::Equipped)
	{
		UnregisterAttachmentNotify(false);
		AttachToUnequipSocket();
	}

	if (IsEquipped())
	{
		OnUnequipped();
	}
}

void UEquipmentController_Weapon::RegisterAttachmentNotify(bool bIsEquip)
{
	UAbilitySystemComponent* AbilitySystem = GetOwnerAbilitySystemComponent();
	if (IsValid(AbilitySystem))
	{
		const FGameplayTag& NotifyTag = UEquipmentSettings::GetNotifyTag(bIsEquip);

		FGameplayEventMulticastDelegate& Delegate = AbilitySystem->GenericGameplayEventCallbacks.FindOrAdd(NotifyTag);
		Delegate.AddUObject(this, &UEquipmentController_Weapon::HandleMontageAttachmentNotify, bIsEquip);
	}

}

void UEquipmentController_Weapon::UnregisterAttachmentNotify(bool bIsEquip)
{
	UAbilitySystemComponent* AbilitySystem = GetOwnerAbilitySystemComponent();
	if (IsValid(AbilitySystem))
	{
		const FGameplayTag& NotifyTag = UEquipmentSettings::GetNotifyTag(bIsEquip);

		FGameplayEventMulticastDelegate& Delegate = AbilitySystem->GenericGameplayEventCallbacks.FindOrAdd(NotifyTag);
		Delegate.RemoveAll(this);
	}
}

void UEquipmentController_Weapon::AttachEquipment()
{
	AttachToUnequipSocket();
}







void UEquipmentController_Skill::HandleActivationEvent(const FGameplayEventData* Payload)
{
	UAbilitySystemComponent* AbilitySystem = GetOwnerAbilitySystemComponent();
	if (!IsValid(AbilitySystem) || !ActiveAbilityHandles.IsValidIndex(0) || !Payload)
	{
		LOG_ERROR(LogTemp, TEXT("AbilitySystem, AbilityHandle, Payload is invalid"));
		return;
	}

	if (!EquipmentTagData || Payload->EventTag != EquipmentTagData->EventTag)
	{
		LOG_WARNING(LogTemp, TEXT("Tag data is invalid or not found"));
		return;
	}

	AbilitySystem->TryActivateAbility(ActiveAbilityHandles[0]);
}

