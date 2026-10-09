// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Blueprint/UserWidget.h"
#include "GameFramework/Actor.h"
#include "UObject/Interface.h"

// Project Headers
#include "GameplayContextAction.h"
#include "RegionActor.h"
#include "StateMachine/FiniteStateMachineType.h"
#include "Core/Type/AssetDetail.h"

// Generated Headers
#include "QuestObjectiveActor.generated.h"

// Forward Declarations
class UEventflowTask;


/*
 *
 */
UCLASS(Abstract)
class AQuestObjectiveActor : public ARegionActor
{
	GENERATED_BODY()
};




DECLARE_DELEGATE_OneParam(FOnInteractionCompleted, EFSMResult);


UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UObjectiveFeedback : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class IObjectiveFeedback
{

	GENERATED_BODY()

public:

	FOnInteractionCompleted& GetOnObjectiveFeedback()
	{
		return OnInteractionCompleted;
	};

protected:

	FOnInteractionCompleted OnInteractionCompleted;

};




/*
 *
 */
UCLASS(Abstract)
class AQuestObjectiveMarker : public ARegionActor
{

	GENERATED_BODY()

public:

	AQuestObjectiveMarker();

	FOnInteractionCompleted OnInteractionCompleted;


protected:

	// ~ ARegionActor
	virtual void HandlePlayerEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	// ~ End of ARegionActor

};




/*
 *
 */
UCLASS(Abstract)
class AObjectiveCascadeMarker : public AQuestObjectiveMarker
{

	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere)
	int Index = 0;

	UPROPERTY(EditAnywhere)
	TArray<FVector> Locations;

protected:

	virtual UPrimitiveComponent* GetCollisionComponent_Implementation() const override;

	void InitialMarkerLocation();
	void UpdateMarkerLocation();

	// ~ ARegionActor
	virtual void HandlePlayerEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void HandlePlayerExited(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex) override;
	// ~ End of ARegionActor

	// ~ AActor
	virtual void BeginPlay() override;
	// ~ End of AActor

};






/*
 *
 */
UCLASS(Abstract)
class AQuestInteractionMarker : public AQuestObjectiveMarker
{
	GENERATED_BODY()
};




/*
 *
 */
USTRUCT(DisplayName = "Quest Interact Handle")
struct FGameplayContext_QuestInteractionHandle : public FGameplayContextAction
{

	GENERATED_BODY()

public:

	// ~ FGameplayContextAction
	virtual bool Execute(UWorld* World, UObject* Owner, UObject* Instigator) override;
	// ~ End of FGameplayContextAction

};




/**
 *
 */
UCLASS(Abstract)
class UQuestInteractUI : public UUserWidget, public IObjectiveFeedback
{

	GENERATED_BODY()

protected:

	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected))
	void InteractionComplete(EFSMResult Result);


	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};


class UAssetCollectionUI;
class UButton;
class UOverlay;

/**
 *
 */
UCLASS(Abstract)
class UQuestInteractInventoryUI : public UQuestInteractUI
{

	GENERATED_BODY()

public:

	UQuestInteractInventoryUI();


	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAssetCollectionUI> InventoryCollection = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAssetCollectionUI> ItemsRequiredCollection = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ConfirmButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CancelButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> MessageOverlay = nullptr;

	UPROPERTY(EditAnywhere)
	FName InventorySourceId;

	UPROPERTY(EditAnywhere)
	FPrimaryAssetType InventoryAssetType;

	UPROPERTY(EditAnywhere)
	TMap<FPrimaryAssetId, int> InventoryItems;


	// ~ Binding
	UFUNCTION()
	void HandleOnConfirmClicked();

	UFUNCTION()
	void HandleOnCancelClicked();
	// ~ End of Binding

	// ~ UUserWidget
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};





/*
 *
 */
USTRUCT()
struct FInstancedTemplateDefinition
{

	GENERATED_BODY()

public:

#if WITH_EDITOR
	virtual bool IsDataValid() const;
	virtual void AppendAssetBundleData(FAssetBundleData& AssetBundle, const FName& Bundle);
#endif

	virtual ~FInstancedTemplateDefinition() = default;

};

/*
 *
 */
USTRUCT()
struct FWidgetTemplateDefinition : public FInstancedTemplateDefinition
{

	GENERATED_BODY()

public:

	UUserWidget* CreateWidget(UWorld* World);

protected:

	virtual TSubclassOf<UUserWidget> GetWidgetClass() const;
	virtual void CopyToInstance(UUserWidget* Widget);

};

/*
 *
 */
USTRUCT(DisplayName = "Quest Inventory UI")
struct FWidgetTemplate_QuestInventory : public FWidgetTemplateDefinition
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UQuestInteractInventoryUI> WidgetClass;

	UPROPERTY(EditAnywhere)
	FName SourceId;

	UPROPERTY(EditAnywhere)
	FPrimaryAssetType AssetType;

	UPROPERTY(EditAnywhere)
	TMap<FPrimaryAssetId, int> Items;


#if WITH_EDITOR
	virtual bool IsDataValid() const override;
	virtual void AppendAssetBundleData(FAssetBundleData& AssetBundle, const FName& Bundle) override;
#endif

protected:

	// ~ FInstancedTemplateDefinition
	virtual TSubclassOf<UUserWidget> GetWidgetClass() const override;
	virtual void CopyToInstance(UUserWidget* Widget) override;
	// ~ End of FInstancedTemplateDefinition

};



