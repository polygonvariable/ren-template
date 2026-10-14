// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"

// Project Headers
#include "Core/QuestQueryDefinition.h"

// Generated Headers
#include "QuestEntryUI.generated.h"

// Forward Declarations
class UAssetManager;
class UTextBlock;
class UImage;


/**
 *
 */
UCLASS(MinimalAPI)
class UQuestEntry : public UObject
{

	GENERATED_BODY()

public:

	FQuestQueryResult QuestResult;

	void Reset()
	{
		QuestResult.Reset();
	}

};


/**
 *
 */
UCLASS(Abstract)
class UQuestEntryUI : public UUserWidget, public IUserObjectListEntry
{

	GENERATED_BODY()

protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> QuestTitle = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> QuestIcon = nullptr;

	UPROPERTY()
	TObjectPtr<UAssetManager> AssetManager;


	// ~ IUserObjectListEntry
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	// ~ End of IUserObjectListEntry

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};

