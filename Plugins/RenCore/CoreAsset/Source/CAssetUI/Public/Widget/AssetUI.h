// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Blueprint/UserWidget.h"

// Project Headers
#include "Core/AssetWidget.h"

// Generated Headers
#include "AssetUI.generated.h"

// Forward Declarations
class UAssetManager;
class UAssetEntry;
class UFragmentedDataAsset;
struct FStreamableHandle;


/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UAssetUI : public UUserWidget, public IAssetWidget
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true))
	FName PrimarySourceId = NAME_None;


	UFUNCTION(BlueprintCallable)
	CASSETUI_API virtual void InitializeDetail() {};

	UFUNCTION(BlueprintCallable)
	CASSETUI_API virtual void CloseWidget();

	CASSETUI_API virtual void InitializeAssetByEntry(const UAssetEntry* Entry);
	CASSETUI_API virtual void InitializeAssetById(const FPrimaryAssetId& AssetId);

	CASSETUI_API virtual void RefreshDetail() {};
	CASSETUI_API virtual void ResetDetail() {};

	// ~ IAssetWidget
	CASSETUI_API virtual void InitializeAssetDetail(const UFragmentedDataAsset* Asset) override;
	CASSETUI_API virtual void InitializeEntryDetail(const UAssetEntry* Entry) override;
	// ~ End of IAssetWidget

protected:

	UPROPERTY()
	TObjectPtr<UAssetManager> AssetManager;


	UFUNCTION(BlueprintNativeEvent)
	CASSETUI_API TArray<UWidget*> GetLockingControls() const;
	CASSETUI_API virtual TArray<UWidget*> GetLockingControls_Implementation() const;

	UFUNCTION(BlueprintNativeEvent)
	CASSETUI_API void LockControls();
	CASSETUI_API virtual void LockControls_Implementation() {};

	UFUNCTION(BlueprintNativeEvent)
	CASSETUI_API void UnlockControls();
	CASSETUI_API virtual void UnlockControls_Implementation() {};

	CASSETUI_API const FPrimaryAssetId& GetActiveAssetId() const;
	CASSETUI_API const UFragmentedDataAsset* GetActiveAsset() const;

	CASSETUI_API virtual void SetPrimaryDetail(const UFragmentedDataAsset* Asset) {};
	CASSETUI_API virtual void SetSecondaryDetail(const UAssetEntry* Entry) {};

	CASSETUI_API virtual void CancelInitialization();
	CASSETUI_API virtual void SwitchDetail(bool bPrimary) {};

	void HandleAssetLoaded();

	// ~ UUserWidget
	CASSETUI_API virtual void NativeConstruct() override;
	CASSETUI_API virtual void NativeDestruct() override;
	// ~ End of UUserWidget

private:

	FPrimaryAssetId _AssetId;
	TSharedPtr<FStreamableHandle> _AssetHandle;

};

