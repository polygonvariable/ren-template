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

	void SetEntryIds(const TArray<FGuid>& NodeIds);

protected:

	TArray<FGuid> EntryIds;

	UQuestAsset* GetQuestAsset() const;
	UQuestSubsystem* GetQuestSubsystem() const;

	// ~ UEventflowEngine
	virtual void HandleOnNodeReached(UEventflowNode* Node) override;
	virtual void HandleOnNodeExited(UEventflowNode* Node, bool bSuccess, int NextNodeIndex) override;
	virtual void HandleOnGraphStarted() override;
	virtual void HandleOnGraphEnded() override;
	// ~ End of UEventflowEngine

};

