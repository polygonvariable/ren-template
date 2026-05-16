// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Forward Declaration
class IDetailsView;
class FEnemyEdMode;
class UEnemyManagerComponent;


/**
 *
 */
class SEnemyEdModeWidget : public SCompoundWidget
{

public:

    SLATE_BEGIN_ARGS(SEnemyEdModeWidget) {}
    SLATE_END_ARGS();

    void Construct(const FArguments& InArgs);
    ~SEnemyEdModeWidget();

protected:

    TSharedPtr<IDetailsView> DetailsView;

    FEnemyEdMode* GetEdMode() const;

    void OnSelectionChanged(UObject* Object);

    void UpdateDebugMode(ECheckBoxState NewState);
    void UpdateManagerObject(UEnemyManagerComponent* Manager);


};

