// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Slate/EnemyEdModeWidget.h"

// Engine Headers
#include "EditorModeManager.h"
#include "Selection.h"
#include "GameFramework/Actor.h"
#include "IDetailsView.h"
#include "PropertyEditorModule.h"

// Project Headers
#include "Component/EnemyManagerComponent.h"
#include "EnemyEdMode.h"


void SEnemyEdModeWidget::Construct(const FArguments& InArgs)
{
    FPropertyEditorModule& PropertyEditorModule = FModuleManager::Get().LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
    FDetailsViewArgs Args;
    {
        Args.bAllowSearch = false;
        Args.bHideSelectionTip = true;
        Args.bLockable = false;
        Args.bSearchInitialKeyFocus = true;
        Args.bUpdatesFromSelection = false;
        Args.NotifyHook = nullptr;
        Args.bShowOptions = true;
        Args.bShowModifiedPropertiesOption = false;
        Args.bShowScrollBar = false;
    }
    
    DetailsView = PropertyEditorModule.CreateDetailView(Args);
    //DetailsView->SetIsPropertyVisibleDelegate(FIsPropertyVisible::CreateLambda([](const FPropertyAndParent& PropertyAndParent)
    //    {
    //        const FProperty& Property = PropertyAndParent.Property;
    //
    //        UClass* OwnerClass = Property.GetOwnerClass();
    //        if (!OwnerClass) return false;
    // 
    //        FName CategoryName = OwnerClass->GetFName();
    //        FString Category = Property.GetMetaData(TEXT("Category"));
    //
    //        return Category == TEXT("Collection") || Category == TEXT("Source");
    //    }
    //));
    DetailsView->SetObject(nullptr);
    
    ChildSlot
        [
            SNew(SVerticalBox)
			    + SVerticalBox::Slot()
                    .Padding(4.0f)
			        .AutoHeight()
			        [
				        SNew(SHorizontalBox)
                            + SHorizontalBox::Slot()
					        .AutoWidth()
                            [
                                SNew(SCheckBox)
                                    .IsChecked(ECheckBoxState::Checked)
                                    .OnCheckStateChanged(this, &SEnemyEdModeWidget::UpdateDebugMode)
                            ]
                            + SHorizontalBox::Slot()
                            [
                                SNew(STextBlock)
						        .Text(FText::FromString(TEXT("Draw Debug")))
                            ]
			        ]
                + SVerticalBox::Slot()
                    .Padding(4.0f)
                    .AutoHeight()
                    [
                        DetailsView.ToSharedRef()
                    ]
        ];

    USelection::SelectionChangedEvent.AddRaw(this, &SEnemyEdModeWidget::OnSelectionChanged);
}

SEnemyEdModeWidget::~SEnemyEdModeWidget()
{
	USelection::SelectionChangedEvent.RemoveAll(this);
}

void SEnemyEdModeWidget::OnSelectionChanged(UObject* Object)
{
	if (GEditor)
	{
        USelection* ActorSelection = GEditor->GetSelectedActors();
        if(ActorSelection->Num() > 0)
		{
			AActor* Actor = Cast<AActor>(ActorSelection->GetSelectedObject(0));
            if (IsValid(Actor))
			{
				UEnemyManagerComponent* Manager = Actor->FindComponentByClass<UEnemyManagerComponent>();
                if (IsValid(Manager))
				{
					UpdateManagerObject(Manager);
                    return;
				}
			}
		}
	}

	UpdateManagerObject(nullptr);
}

void SEnemyEdModeWidget::UpdateManagerObject(UEnemyManagerComponent* Manager)
{
    DetailsView->SetObject(Manager);
    DetailsView->RefreshRootObjectVisibility();
}

void SEnemyEdModeWidget::UpdateDebugMode(ECheckBoxState NewState)
{
    bool bChecked = (NewState == ECheckBoxState::Checked) ? true : false;

    FEnemyEdMode* Mode = GetEdMode();
    if (Mode)
    {
        Mode->bDrawDebug = bChecked;
    }
}

FEnemyEdMode* SEnemyEdModeWidget::GetEdMode() const
{
    return (FEnemyEdMode*)(GLevelEditorModeTools().GetActiveMode(FEnemyEdMode::EM_EnemyManager));
}

