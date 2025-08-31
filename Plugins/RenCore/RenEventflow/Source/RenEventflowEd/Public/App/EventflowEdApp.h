// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"

// Project Headers

// Generated Headers

// Forward Declarations
class UEventflowAsset;

class UEventflowEdGraph;
class UEventflowEdGraphNode;
class UEventflowEdGraphSchema;



class RENEVENTFLOWED_API FEventflowEdApp : public FWorkflowCentricApplication, public FEditorUndoClient, public FNotifyHook
{

public:

	void InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UObject* ObjectToEdit);

	UEventflowAsset* GetWorkingAsset() const;
	UEventflowEdGraph* GetWorkingGraph() const;

	void SetGraphEditorSlate(TSharedPtr<SGraphEditor> InGraphEditor);
	void SetGraphPropertySlate(TSharedPtr<IDetailsView> InDetailsView);
	void SetNodePropertySlate(TSharedPtr<IDetailsView> InDetailsView);

	virtual void RegisterGraphEditorEvents(SGraphEditor::FGraphEditorEvents& GraphEvents);

protected:

	virtual void OnGraphSelectionChanged(const FGraphPanelSelectionSet& SelectedNodes);
	virtual void OnGraphNodeDoubleClicked(UEdGraphNode* Node);

	virtual void OnGraphPropertyChanged(const FPropertyChangedEvent& PropertyChangedEvent);
	virtual void OnNodePropertyChanged(const FPropertyChangedEvent& PropertyChangedEvent);

	virtual TArray<FName> GetTriggerGraphProperties() const;
	virtual TArray<FName> GetTriggerNodeProperties() const;

	void UpdateWorkingAsset();

	UEventflowEdGraphNode* GetFirstSelectedNode(const FGraphPanelSelectionSet& SelectedNodes) const;

	virtual TSubclassOf<UEventflowEdGraph> GetGraphClass() const;
	virtual TSubclassOf<UEventflowEdGraphSchema> GetGraphSchemaClass() const;

public:

	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;

	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;

	virtual void OnClose() override;

protected:

	UPROPERTY()
	UEventflowAsset* WorkingAsset = nullptr;

	UPROPERTY()
	UEventflowEdGraph* WorkingGraph = nullptr;

	TSharedPtr<SGraphEditor> GraphEditorSlate;
	TSharedPtr<IDetailsView> GraphPropertySlate;
	TSharedPtr<IDetailsView> NodePropertySlate;

	FDelegateHandle EventflowAssetSaved;
	FDelegateHandle GraphChangedHandle;

};