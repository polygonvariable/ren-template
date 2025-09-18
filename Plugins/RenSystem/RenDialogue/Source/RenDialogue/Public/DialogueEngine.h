// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenEventflow/Public/EventflowEngine.h"

// Generated Headers
#include "DialogueEngine.generated.h"

// Forward Declarations
class UDialogueWidget;



/**
 *
 */
UCLASS()
class UDialogueEngine : public UEventflowEngine
{

	GENERATED_BODY()

protected:

	TObjectPtr<UDialogueWidget> DialogueWidget;
	
	// ~ UEventflowEngine
	virtual void HandleOnNodeReached(UEventflowNode* Node) override;
	virtual void HandleOnNodeExited(UEventflowNode* Node, bool bSuccess, int NextNodeIndex) override;
	virtual void HandleOnGraphStarted() override;
	virtual void HandleOnGraphEnded() override;
	// ~ End of UEventflowEngine

};

