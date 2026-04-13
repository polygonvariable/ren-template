// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "ActiveGameplayEffectHandle.h"
#include "GameFramework/Actor.h"
#include "GameplayAbilitySpecHandle.h"

// Project Headers
#include "Definition/EquipmentData.h"
#include "Interface/IActorLinkedNode.h"

// Generated Headers
#include "EquipmentActor.generated.h"

// Forward Declarations
class UGameplayEffect;
class UGameplayAbility;
class UCoreDataAsset;
class IAssetInstanceCollection;
class IAscensionInstanceProvider;
struct FGameplayEventData;


/**
 * 
 */
UCLASS(Abstract)
class AEquipmentActor : public AActor, public IActorLinkedNode
{

	GENERATED_BODY()

public:

	AEquipmentActor(const FObjectInitializer& ObjectInitializer);


	UPROPERTY(VisibleAnywhere, AdvancedDisplay)
	TObjectPtr<const UCoreDataAsset> EquipmentAsset;

	UPROPERTY(VisibleAnywhere, AdvancedDisplay)
	FEquipmentData EquipmentData;

	UPROPERTY(VisibleAnywhere, AdvancedDisplay)
	FGuid EquipmentOwnerId;




	void InitializeEquipment();
	void DeinitializeEquipment();


	UFUNCTION(BlueprintCallable)
	bool GetIsEquipped() const;

	UFUNCTION(BlueprintCallable)
	void SetIsEquipped(bool IsEquipped);


	// ~ IActorLinkedNode
	virtual AActor* GetNextNode() const override final;
	virtual void SetNextNode(AActor* Node) override final;
	// ~ End of IActorLinkedNode

	// ~ AActor
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// ~ End of AActor

protected:

	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> ActiveEffectHandles;

	UPROPERTY()
	FGameplayAbilitySpecHandle ActiveAbilityHandle;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAbilitySystemComponent> OwnerASC = nullptr;





	IAssetInstanceCollection* InstanceCollection = nullptr;
	IAscensionInstanceProvider* InstanceAscension = nullptr;


	void HandleActivationEvent(const FGameplayEventData* Payload);


	void ApplyEffects();
	void RemoveEffects();
	void RefreshEffects();


	void RefreshEquipment();
	void PostInitializeEquipment();


	UFUNCTION(BlueprintNativeEvent)
	void AttachEquipment();
	virtual void AttachEquipment_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void DetachEquipment();
	virtual void DetachEquipment_Implementation();


	UFUNCTION(BlueprintCallable)
	int GetEquipmentLevel() const;
	void SetEquipmentLevel(int Level);


	void OnEquipped();
	void OnUnequipped();



	UFUNCTION(BlueprintNativeEvent, Meta = (DisplayName = "Initialize Equipment"))
	void BP_InitializeEquipment();
	virtual void BP_InitializeEquipment_Implementation() {};

	UFUNCTION(BlueprintNativeEvent, Meta = (DisplayName = "Deinitialize Equipment"))
	void BP_DeinitializeEquipment();
	virtual void BP_DeinitializeEquipment_Implementation() {};

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Meta = (DisplayName = "On Equipped"))
	void BP_OnEquipped();
	virtual void BP_OnEquipped_Implementation() {};

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Meta = (DisplayName = "On Unequipped"))
	void BP_OnUnequipped();
	virtual void BP_OnUnequipped_Implementation() {};


	// ~ UAttributeSet
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// ~ End of UAttributeSet

private:

	UPROPERTY()
	bool _bIsEquipped = false;

	UPROPERTY()
	AActor* _NextNode = nullptr;

	UPROPERTY(VisibleAnywhere, AdvancedDisplay)
	int _EquipmentLevel = 1;

};

