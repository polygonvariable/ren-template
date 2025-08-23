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
class UEventflowEdGraphNode;


class EventflowEdApp : public FWorkflowCentricApplication, public FEditorUndoClient, public FNotifyHook
{

public:

	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	void InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UObject* ObjectToEdit);

	UEventflowAsset* GetWorkingAsset() const { return WorkingAsset; }
	UEdGraph* GetWorkingGraph() const { return WorkingGraph; }

	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;


	virtual void OnClose() override;
	void OnGraphChanged(const FEdGraphEditAction& InAction);
	void OnNodeDetailsChanged(const FPropertyChangedEvent& PropertyChangedEvent);


	void SetWorkingGraphEditor(TSharedPtr<SGraphEditor> InGraphEditor);
	void SetSelectedNodeDetail(TSharedPtr<IDetailsView> InDetailsView);

	void OnGraphSelectionChanged(const FGraphPanelSelectionSet& SelectedNodes);

	UEventflowEdGraphNode* GetSelectedNode(const FGraphPanelSelectionSet& SelectedNodes) const;


	//virtual void OnToolkitHostingStarted(const TSharedRef<IToolkitHost>& InToolkitHost) override {};
	//virtual void OnToolkitHostingFinished(const TSharedRef<IToolkitHost>& InToolkitHost) override {};

protected:

	void UpdateWorkingAssetFromGraph();
	void UpdateEditorGraphFromWorkingAsset();

private:

	TSharedPtr<SGraphEditor> WorkingGraphEditor;
	TSharedPtr<IDetailsView> SelectedNodeDetail;


	UPROPERTY()
	UEventflowAsset* WorkingAsset = nullptr;


	UPROPERTY()
	UEdGraph* WorkingGraph = nullptr;


	FDelegateHandle GraphChangedHandle;

};