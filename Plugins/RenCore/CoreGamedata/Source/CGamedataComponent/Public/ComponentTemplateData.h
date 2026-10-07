// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Generated Headers
#include "ComponentTemplateData.generated.h"


/*
 *
 */
USTRUCT()
struct CGAMEDATACOMPONENT_API FComponentTemplateData
{

	GENERATED_BODY()

public:

    virtual bool IsValid() const;
    virtual bool AttachToParent(UActorComponent* Target, AActor* Owner);
    virtual bool ApplyToInstance(UObject* Target);
    virtual TSubclassOf<UActorComponent> GetComponentClass() const;
    
	virtual ~FComponentTemplateData() = default;
    
};


/*
 *
 */
USTRUCT(DisplayName = "Scene Component")
struct CGAMEDATACOMPONENT_API FSceneComponentTemplateData : public FComponentTemplateData
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FName NativeParent = TEXT_EMPTY;

	UPROPERTY(EditAnywhere)
	FName NativeParentSocket = TEXT_EMPTY;

    // ~ FComponentTemplateData
    virtual bool AttachToParent(UActorComponent* Target, AActor* Owner) override;
    // ~ End of FComponentTemplateData

};

