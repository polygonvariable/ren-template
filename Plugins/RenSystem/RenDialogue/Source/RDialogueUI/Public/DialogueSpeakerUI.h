// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Blueprint/UserWidget.h"

// Project Headers
#include "DialogueData.h"

// Generated Headers
#include "DialogueSpeakerUI.generated.h"

// Forward Declarations
class UImage;


/**
 *
 */
UCLASS(Abstract)
class UDialogueSpeakerUI : public UUserWidget
{

	GENERATED_BODY()

public:

	FDialogueSpeaker Speaker;
	

	void SetActive(bool bActive);
	void SetImage(TSoftObjectPtr<UTexture2D> Image);

protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> SpeakerImage = nullptr;


	// ~ UUserWidget
	virtual void NativeConstruct() override;
	// ~ End of UUserWidget
};

