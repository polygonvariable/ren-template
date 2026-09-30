// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/WorldSubsystem.h"

// Project Headers
#include "StateMachine/FiniteStateMachineType.h"

// Generated Headers
#include "DialogueSubsystem.generated.h"

// Forward Declarations
class UDialogueEngine;


/**
 *
 */
UCLASS(MinimalAPI)
class UDialogueSubsystem : public UWorldSubsystem
{

	GENERATED_BODY()

public:

	DECLARE_DELEGATE_TwoParams(FOnDialogueAdded, FPrimaryAssetId /* AssetId */, UDialogueEngine* /* Engine */);
	FOnDialogueAdded OnDialogueAdded;

	DECLARE_DELEGATE_OneParam(FOnDialogueRemoved, FPrimaryAssetId /* AssetId */);
	FOnDialogueRemoved OnDialogueRemoved;


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

