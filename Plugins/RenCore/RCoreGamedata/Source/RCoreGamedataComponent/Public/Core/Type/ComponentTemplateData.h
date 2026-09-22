// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Generated Headers
#include "ComponentTemplateData.generated.h"


/*
 *
 */
USTRUCT()
struct FComponentTemplateData
{

	GENERATED_BODY()

public:

    RCOREGAMEDATACOMPONENT_API virtual bool IsDataValid(UClass* Target) const;
    RCOREGAMEDATACOMPONENT_API virtual bool AttachToParent(UActorComponent* Target, AActor* Owner);
    RCOREGAMEDATACOMPONENT_API virtual bool ApplyToInstance(UObject* Target);
    
	virtual ~FComponentTemplateData() = default;
    
};


/*
 *
 */
USTRUCT(DisplayName = "Scene Component")
struct FSceneComponentTemplateData : public FComponentTemplateData
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FName NativeParent = TEXT_EMPTY;

	UPROPERTY(EditAnywhere)
	FName NativeParentSocket = TEXT_EMPTY;

    // ~ FComponentTemplateData
    RCOREGAMEDATACOMPONENT_API virtual bool AttachToParent(UActorComponent* Target, AActor* Owner) override;
    // ~ End of FComponentTemplateData

};

