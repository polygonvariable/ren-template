// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Blueprint/UserWidget.h"

// Generated Headers
#include "AscensionDetailUI.generated.h"

// Module Macros
#define REN_API RCOREASCENSIONUI_API

// Forward Declarations
class UTextBlock;
struct FAscensionData;


/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UAscensionDetailUI : public UUserWidget
{

	GENERATED_BODY()

public:
	
    REN_API void InitializeDetail(const FAscensionData& Data);

protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ExperienceText = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> LevelText = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> RankText = nullptr;
	
};


// Module Macros
#undef REN_API

