// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "InteractItem.h"

// Generated Headers
#include "InteractActor.generated.h"

// Forward Declarations
class UInteractComponent;


/**
 *
 */
UCLASS(Abstract, DisplayName = "Interact Actor")
class RENINTERACT_API AInteractActor : public AActor
{

	GENERATED_BODY()
	
public:

	AInteractActor();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact Actor|Item")
	FInteractItem InteractItem;


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Meta = (ForceAsFunction), Category = "Interact Actor|Callback")
	void Interact();
	virtual void Interact_Implementation();

protected:

	UPROPERTY(BlueprintReadWrite, Category = "Interact Actor|Runtime")
	UInteractComponent* InteractComponent;


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Meta = (ForceAsFunction, BlueprintProtected, BlueprintPure), Category = "Interact Actor|Action")
	UInteractComponent* GetInteractComponent();
	virtual UInteractComponent* GetInteractComponent_Implementation();


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Meta = (ForceAsFunction, BlueprintProtected), Category = "Interact Actor|Action")
	void StartInteract();
	virtual void StartInteract_Implementation();


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Meta = (ForceAsFunction, BlueprintProtected), Category = "Interact Actor|Action")
	void EndInteract();
	virtual void EndInteract_Implementation();


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Meta = (ForceAsFunction, BlueprintProtected), Category = "Interact Actor|Action")
	void UpdateItem();
	virtual void UpdateItem_Implementation();


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Meta = (ForceAsFunction, BlueprintProtected), Category = "Interact Actor|Handler")
	void OnInteracted();
	virtual void OnInteracted_Implementation();

protected:

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemUpdated, FInteractItem, Item);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Interact Actor|Event Dispatcher")
	FOnItemUpdated OnItemUpdated;

};

