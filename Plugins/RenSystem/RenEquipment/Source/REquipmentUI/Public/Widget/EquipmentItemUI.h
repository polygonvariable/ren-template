// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Blueprint/UserWidget.h"
#include "Core/Type/EquipmentSlotDefinition.h"

// Generated Headers
#include "EquipmentItemUI.generated.h"

// Forward Declarations
class UImage;
class UOverlay;
class UTextBlock;
class UEquipmentManagerComponent;
class UEquipmentController;


/**
 *
 */
UCLASS(Abstract)
class UEquipmentItemUI : public UUserWidget
{

	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UOverlay> EquipmentOverlay = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> EquipmentImage = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> KeyTextBlock = nullptr;

	UPROPERTY(EditAnywhere)
	FText KeyText;

	UPROPERTY(EditAnywhere)
	FEquipmentSlotDefinition SlotDefinition;


	UEquipmentManagerComponent* GetEquipmentComponent() const;
	UEquipmentController* GetEquipmentController() const;

	template<class T>
	T* GetEquipmentComponent() const
	{
		return Cast<T>(_EquipmentComponent.Get());
	}

	template<class T>
	T* GetEquipmentController() const
	{
		return Cast<T>(_EquipmentController.Get());
	}

	virtual void RegisterEquipmentComponent(AActor* Target);
	virtual void UnregisterEquipmentComponent();

	virtual void RegisterEquipmentController();
	virtual void UnregisterEquipmentController();

	virtual void SetDetail(UEquipmentController* Controller);
	virtual void RefreshDetail();
	virtual void ResetDetail();

	void RegisterPlayer();
	void UnregisterPlayer();

	// ~ Bindings
	virtual void OnPlayerRegistered(AActor* Target);
	virtual void OnPlayerUnregistered();
	// ~ End of Bindings

	// ~ UUserWidget
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

private:

	UPROPERTY()
	TWeakObjectPtr<UEquipmentManagerComponent> _EquipmentComponent = nullptr;

	UPROPERTY()
	TWeakObjectPtr<UEquipmentController>  _EquipmentController = nullptr;

};

