// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Blueprint/UserWidget.h"
#include "Core/Type/EquipmentSlotId.h"

// Generated Headers
#include "EquipmentItemUI.generated.h"

// Module Macros
#define REN_API REQUIPMENTUI_API

// Forward Declarations
class UImage;
class UOverlay;
class UTextBlock;
class UEquipmentManagerComponent;
class UEquipmentController;


/**
 *
 */
UCLASS(Abstract, MinimalAPI)
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
	FEquipmentSlotId SlotId;


	REN_API UEquipmentManagerComponent* GetEquipmentComponent() const;
	REN_API UEquipmentController* GetEquipmentController() const;

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

	void RegisterEquipmentComponent(AActor* Target);
	void UnregisterEquipmentComponent();

	REN_API virtual void RegisterEquipmentController();
	REN_API virtual void UnregisterEquipmentController();

	REN_API virtual void SetDetail(UEquipmentController* Controller);
	REN_API virtual void RefreshDetail();
	REN_API virtual void ResetDetail();

	void RegisterPlayer();
	void UnregisterPlayer();

	// ~ Bindings
	REN_API virtual void OnPlayerRegistered(AActor* Target);
	REN_API virtual void OnPlayerUnregistered();
	// ~ End of Bindings

	// ~ UUserWidget
	REN_API virtual void NativePreConstruct() override;
	REN_API virtual void NativeConstruct() override;
	REN_API virtual void NativeDestruct() override;
	// ~ End of UUserWidget

private:

	UPROPERTY()
	TWeakObjectPtr<UEquipmentManagerComponent> _EquipmentComponent = nullptr;

	UPROPERTY()
	TWeakObjectPtr<UEquipmentController>  _EquipmentController = nullptr;

};

// Module Macros
#undef REN_API

