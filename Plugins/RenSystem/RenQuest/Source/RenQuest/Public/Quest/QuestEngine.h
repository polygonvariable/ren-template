// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenEventflow/Public/EventflowEngine.h"

// Generated Headers
#include "QuestEngine.generated.h"

// Forward Declarations
class UQuestAsset;
class UQuestSubsystem;



/**
 *
 * 
 */
UCLASS()
class UQuestEngine : public UEventflowEngine
{

	GENERATED_BODY()

public:

	void SetEntryId(FGuid NodeId);

protected:

	FGuid EntryId;

	UQuestAsset* GetQuestAsset() const;
	UQuestSubsystem* GetQuestSubsystem() const;

	// ~ UEventflowEngine
	virtual void HandleOnNodeReached(UEventflowNode* Node) override;
	virtual void HandleOnNodeExited(UEventflowNode* Node, bool bSuccess) override;
	virtual void HandleOnGraphEnded() override;
	virtual void HandleOnEngineInitialized() override;
	// ~ End of UEventflowEngine

};

