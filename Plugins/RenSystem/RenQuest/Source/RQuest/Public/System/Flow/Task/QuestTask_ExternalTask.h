// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "System/Flow/Task/QuestPrimaryTask.h"

// Generated Headers
#include "QuestTask_ExternalTask.generated.h"

// Forward Declarations
class UEventflowAsset;
class UEventflowEngine;


/**
 *
 */
UCLASS(MinimalAPI)
class UQuestTask_ExternalTask : public UQuestPrimaryTask
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "External Asset")
	TSoftObjectPtr<UEventflowAsset> ExternalAsset;

	UPROPERTY(VisibleAnywhere, Category = "External Asset")
	TArray<FGuid> ExternalPins;


	// ~ UEventflowTask
	virtual void CopyFromAsset(const UEventflowTask* Template) override;
	// ~ End of UEventflowTask

#if WITH_EDITOR
	// ~ UEventflowTask
	virtual void AppendAssetBundleData(FAssetBundleData& AssetBundle) override;
	// ~ End of UEventflowTask

	// ~ UObject
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
	// ~ End of UObject
#endif

protected:

	FPrimaryAssetId GetGroupId() const;

	// ~ Binding
	void HandleOnEngineRemoved(FPrimaryAssetId AssetId, UEventflowEngine* Engine);
	// ~ End of Binding

	// ~ UEventflowTask
	virtual void OnInitialized(EFSMState PreviousState) override;
	virtual void OnLoaded(EFSMState PreviousState) override;
	virtual void OnReady(EFSMState PreviousState) override;
	virtual void OnActive(EFSMState PreviousState) override;
	virtual void OnReset() override;
	// ~ End of UEventflowTask

};

