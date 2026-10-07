// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/WorldSubsystem.h"

// Project Headers
#include "EventflowEngineProvider.h"
#include "StateMachine/FiniteStateMachineType.h"

// Generated Headers
#include "DialogueSubsystem.generated.h"

// Forward Declarations
class UDialogueEngine;
class UEventflowEngine;


/**
 *
 */
UCLASS(MinimalAPI)
class UDialogueSubsystem : public UWorldSubsystem, public IEventflowEngineProvider
{

	GENERATED_BODY()

public:

	// ~ IEventflowEngineProvider
	virtual void StartEventflow(const FPrimaryAssetId& AssetId) override;
	virtual void StopEventflow(const FPrimaryAssetId& AssetId) override;
	// ~ End of IEventflowEngineProvider

	UFUNCTION(BlueprintCallable)
	void StartDialogue(const FPrimaryAssetId& AssetId);

#if UE_BUILD_DEVELOPMENT
	RDIALOGUE_API const TMap<FPrimaryAssetId, TObjectPtr<UDialogueEngine>>& GetEditorDialogues() const;
	RDIALOGUE_API int GetEditorDialoguePoolSize() const;
#endif

protected:

	UPROPERTY()
	TMap<FPrimaryAssetId, TObjectPtr<UDialogueEngine>> Dialogues;


	UDialogueEngine* GetDialogueEngine(FPrimaryAssetId AssetId) const;

	// ~ Binding
	void HandleOnEngineStateChanged(EFSMState PreviousState, EFSMState NewState, EFSMResult Result, FPrimaryAssetId AssetId);
	// ~ End of Binding

	// ~ UWorldSubsystem
	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	// ~ End of UWorldSubsystem

private:

	UPROPERTY()
	TArray<TObjectPtr<UDialogueEngine>> EnginePool;

public:

	static RDIALOGUE_API UDialogueSubsystem* Get(UWorld* World);

};

