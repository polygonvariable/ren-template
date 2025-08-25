// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"
#include "SGraphPanel.h"

// Project Headers

// Generated Headers

// Forward Declarations
class UEventflowAsset;

class UEventflowEdGraph;
class UEventflowEdGraphNode;
class UEventflowEdGraphSchema;



class FEventflowEdApp : public FWorkflowCentricApplication, public FEditorUndoClient, public FNotifyHook
{

public:

	void SetWorkingGraphEditor(TSharedPtr<SGraphEditor> InGraphEditor);
	void SetSelectedNodeDetail(TSharedPtr<IDetailsView> InDetailsView);

	void OnGraphSelectionChanged(const FGraphPanelSelectionSet& SelectedNodes);

protected:

	void OnGraphChanged(const FEdGraphEditAction& InAction);
	void OnNodeDetailsChanged(const FPropertyChangedEvent& PropertyChangedEvent);

	void UpdateWorkingAssetFromGraph();
	void UpdateEditorGraphFromWorkingAsset();

	UEventflowEdGraphNode* GetSelectedNode(const FGraphPanelSelectionSet& SelectedNodes) const;

	virtual void RegisterGraphNodes();

	virtual TSubclassOf<UEventflowEdGraph> GetGraphClass() const;
	virtual TSubclassOf<UEventflowEdGraphSchema> GetGraphSchemaClass() const;

public:

	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	void InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UObject* ObjectToEdit);

	UEventflowAsset* GetWorkingAsset() const { return WorkingAsset; }
	UEventflowEdGraph* GetWorkingGraph() const { return WorkingGraph; }

	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;


	virtual void OnClose() override;

private:

	TSharedPtr<SGraphEditor> WorkingGraphEditor;
	TSharedPtr<IDetailsView> SelectedNodeDetail;


	UPROPERTY()
	UEventflowAsset* WorkingAsset = nullptr;

	UPROPERTY()
	UEventflowEdGraph* WorkingGraph = nullptr;


	FDelegateHandle EventflowAssetSaved;
	FDelegateHandle GraphChangedHandle;

};