// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Generated Headers
#include "WidgetTemplateDefinition.generated.h"

// Forward Declarations
class UUserWidget;


/*
 *
 */
USTRUCT()
struct CGAMEDATAWIDGET_API FWidgetTemplateDefinition
{

	GENERATED_BODY()

public:

	UUserWidget* CreateWidget(UWorld* World);

#if WITH_EDITOR
	virtual bool IsDataValid() const;
	virtual void AppendAssetBundleData(FAssetBundleData& AssetBundle, const FName& Bundle);
#endif

	virtual ~FWidgetTemplateDefinition() = default;

protected:

	virtual TSubclassOf<UUserWidget> GetWidgetClass() const;
	virtual void InitializeInstance(UUserWidget* Widget);

};

