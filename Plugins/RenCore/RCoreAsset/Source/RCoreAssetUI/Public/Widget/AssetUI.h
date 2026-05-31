// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Blueprint/UserWidget.h"

// Project Headers
#include "Interface/AssetWidget.h"

// Generated Headers
#include "AssetUI.generated.h"

// Module Macros
#define REN_API RCOREASSETUI_API

// Forward Declarations
class UAssetManager;
class UAssetEntry;
class UCoreDataAsset;
struct FStreamableHandle;


/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UAssetUI : public UUserWidget, public IAssetWidget
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (ExposeOnSpawn = true))
	FName PrimarySourceId = NAME_None;


	UFUNCTION(BlueprintCallable)
	REN_API virtual void InitializeDetail() {};

	UFUNCTION(BlueprintCallable)
	REN_API virtual void CloseWidget();

	REN_API virtual void InitializeAssetByEntry(const UAssetEntry* Entry);
	REN_API virtual void InitializeAssetById(const FPrimaryAssetId& AssetId);

	REN_API virtual void RefreshDetail() {};
	REN_API virtual void ResetDetail() {};

	// ~ IAssetWidget
	REN_API virtual void InitializeAssetDetail(const UCoreDataAsset* Asset) override;
	REN_API virtual void InitializeEntryDetail(const UAssetEntry* Entry) override;
	// ~ End of IAssetWidget

protected:

	UPROPERTY()
	TObjectPtr<UAssetManager> AssetManager;


	UFUNCTION(BlueprintNativeEvent)
	REN_API TArray<UWidget*> GetLockingControls() const;
	REN_API virtual TArray<UWidget*> GetLockingControls_Implementation() const;

	UFUNCTION(BlueprintNativeEvent)
	REN_API void LockControls();
	REN_API virtual void LockControls_Implementation() {};

	UFUNCTION(BlueprintNativeEvent)
	REN_API void UnlockControls();
	REN_API virtual void UnlockControls_Implementation() {};

	REN_API const FPrimaryAssetId& GetActiveAssetId() const;
	REN_API const UCoreDataAsset* GetActiveAsset() const;

	REN_API virtual void SetPrimaryDetail(const UCoreDataAsset* Asset) {};
	REN_API virtual void SetSecondaryDetail(const UAssetEntry* Entry) {};

	REN_API virtual void CancelInitialization();
	REN_API virtual void SwitchDetail(bool bPrimary) {};

	void HandleAssetLoaded();

	// ~ UUserWidget
	REN_API virtual void NativeConstruct() override;
	REN_API virtual void NativeDestruct() override;
	// ~ End of UUserWidget

private:

	FPrimaryAssetId _AssetId;
	TSharedPtr<FStreamableHandle> _AssetHandle;

};


// Module Macros
#undef REN_API

