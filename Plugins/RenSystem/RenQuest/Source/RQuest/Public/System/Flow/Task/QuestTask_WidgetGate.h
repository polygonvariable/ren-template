// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "WidgetTemplateDefinition.h"
#include "System/Flow/Task/QuestPrimaryTask.h"

// Generated Headers
#include "QuestTask_WidgetGate.generated.h"

// Forward Declarations
class UUserWidget;


/**
 *
 */
UCLASS(MinimalAPI)
class UQuestTask_WidgetGate : public UQuestPrimaryTask
{

	GENERATED_BODY()

public:

	// ~ Binding
	virtual void CopyFromAsset(const UEventflowTask* Template) override;
	// ~ End of Binding

#if WITH_EDITOR
	// ~ UEventflowTask
	virtual void AppendAssetBundleData(FAssetBundleData& AssetBundle) override;
	// ~ End of UEventflowTask
	// ~ UObject
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
	// ~ End of UObject
#endif

protected:

	UPROPERTY(EditAnywhere)
	FName WidgetGameplayMode;

	UPROPERTY(EditAnywhere, meta = (ExcludeBaseStruct))
	TInstancedStruct<FWidgetTemplateDefinition> WidgetTemplate;

	UPROPERTY()
	TObjectPtr<UUserWidget> Widget;

	// ~ Binding
	virtual void HandleOnObjectiveFeedback(EFSMResult Result);
	// ~ End of Binding

	// ~ UEventflowTask
	virtual void OnLoaded(EFSMState PreviousState) override;
	virtual void OnActive(EFSMState PreviousState) override;
	virtual void OnReset() override;
	// ~ End of UEventflowTask
};

