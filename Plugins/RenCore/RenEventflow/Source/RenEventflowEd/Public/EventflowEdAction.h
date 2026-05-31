// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "AssetTypeActions_Base.h"


/*
 *
 */
class FEventflowEdAction : public FAssetTypeActions_Base
{

public:

	FEventflowEdAction(EAssetTypeCategories::Type InAssetCategory);

	// ~ FAssetTypeActions_Base
	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual uint32 GetCategories() override;
	virtual void OpenAssetEditor(const TArray<UObject*>& Objects, TSharedPtr<IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;
	// ~ End of FAssetTypeActions_Base

private:

	EAssetTypeCategories::Type AssetCategory;

};

