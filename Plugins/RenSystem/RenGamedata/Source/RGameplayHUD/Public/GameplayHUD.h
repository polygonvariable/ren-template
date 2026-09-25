// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameFramework/HUD.h"
#include "GameplayTagContainer.h"

// Generated Headers
#include "GameplayHUD.generated.h"

// Forward Declaration
class IGameplayModeProvider;
class UUserWidget;
struct FStreamableHandle;


/**
 * 
 */
UCLASS(MinimalAPI)
class AGameplayHUD : public AHUD
{

	GENERATED_BODY()

public:

	AGameplayHUD();

	// ~ APlayerController
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// ~ End of APlayerController

#if WITH_EDITOR
	// ~ UActorComponent
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
	// ~ End of UActorComponent
#endif

protected:

	UPROPERTY(EditAnywhere, meta = (Categories = "Gameplay"))
	FGameplayTag HUDVisiblityTag = FGameplayTag::RequestGameplayTag(TEXT("Gameplay.HUD.Visible"));

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UUserWidget> HUDWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UUserWidget> HUDWidget;

	IGameplayModeProvider* GameplayMode;


	// ~ Binding
	void HandleOnAssetLoaded();
	void HandleOnHUDVisiblityTagChanged(bool bAdded);
	// ~ End of Binding

private:

	TSharedPtr<FStreamableHandle> AssetHandle;

};

