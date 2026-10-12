// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Generated Headers
#include "ComponentTemplateDefinition.generated.h"


/*
 *
 */
USTRUCT()
struct CGAMEDATACOMPONENT_API FComponentTemplateDefinition
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	bool bModifyExisting = false;

	UActorComponent* CreateComponent(AActor* Owner);
    
#if WITH_EDITOR
	virtual bool IsDataValid() const;
	virtual void AppendAssetBundleData(FAssetBundleData& AssetBundle, const FName& Bundle);
#endif

	virtual ~FComponentTemplateDefinition() = default;

protected:

	virtual TSubclassOf<UActorComponent> GetComponentClass() const;
	virtual bool AttachToParent(UActorComponent* Target, AActor* Owner);
	virtual void InitializeInstance(UActorComponent* Target);
    
};


/*
 *
 */
USTRUCT(DisplayName = "Scene Component")
struct CGAMEDATACOMPONENT_API FSceneComponentTemplateDefinition : public FComponentTemplateDefinition
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FName NativeParent = TEXT_EMPTY;

	UPROPERTY(EditAnywhere)
	FName NativeParentSocket = TEXT_EMPTY;

#if WITH_EDITOR
	// ~ FComponentTemplateDefinition
	virtual bool IsDataValid() const;
	// ~ End of FComponentTemplateDefinition
#endif

protected:

    // ~ FComponentTemplateDefinition
	virtual TSubclassOf<UActorComponent> GetComponentClass() const override;
    virtual bool AttachToParent(UActorComponent* Target, AActor* Owner) override;
    // ~ End of FComponentTemplateDefinition

};

