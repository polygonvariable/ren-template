// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Generated Headers
#include "DialogueManagerComponent.generated.h"

// Forward Declarations
class UEventflowEngine;
class UDialogueUI;


/**
 * Component to responds to DialogueSubsystem and manages dialogue widgets,
 * must be placed in HUD class
 */
UCLASS(MinimalAPI, meta = (BlueprintSpawnableComponent))
class UDialogueManagerComponent : public UActorComponent
{

	GENERATED_BODY()

public:

	UDialogueManagerComponent();

	// ~ UActorComponent
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// ~ End of UActorComponent

protected:

	UPROPERTY(VisibleAnywhere)
	TMap<FPrimaryAssetId, TObjectPtr<UDialogueUI>> WidgetCollection;


	APlayerController* GetPlayerController() const;

	// ~ Binding
	void HandleOnDialogueAdded(FPrimaryAssetId AssetId, UEventflowEngine* Engine);
	void HandleOnDialogueRemoved(FPrimaryAssetId AssetId, UEventflowEngine* Engine);
	// ~ End of Binding

};

