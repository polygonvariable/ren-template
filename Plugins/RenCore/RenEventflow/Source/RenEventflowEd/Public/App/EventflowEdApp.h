// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"

// Module Macros
#define REN_API RENEVENTFLOWED_API

// Forward Declarations
class UEventflowAsset;
class UEventflowEdGraph;
class UEventflowEdGraphNode;
class UEventflowEdGraphSchema;


/*
 * 
 */
class REN_API FEventflowEdApp : public FWorkflowCentricApplication, public FEditorUndoClient, public FNotifyHook
{

public:

	void InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UObject* ObjectToEdit);

	UEventflowAsset* GetWorkingAsset() const;
	UEventflowEdGraph* GetWorkingGraph() const;

	void RegisterGraphEvent(SGraphEditor::FGraphEditorEvents& GraphEvents);
	void RegisterGraphEditor(TSharedPtr<SGraphEditor> GraphEditor);
	void RegisterGraphProperty(TSharedPtr<IDetailsView> DetailsView);
	void RegisterNodeProperty(TSharedPtr<IDetailsView> DetailsView);

	// ~ FAssetEditorToolkit
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	// ~ End of FAssetEditorToolkit

	// ~ FWorkflowCentricApplication
	virtual void OnClose() override;
	// ~ End of FWorkflowCentricApplication

protected:

	UPROPERTY()
	TObjectPtr<UEventflowAsset>  WorkingAsset = nullptr;

	UPROPERTY()
	TObjectPtr<UEventflowEdGraph> WorkingGraph = nullptr;

	TSharedPtr<SGraphEditor> GraphEditorSlate = nullptr;
	TSharedPtr<IDetailsView> GraphPropertySlate = nullptr;
	TSharedPtr<IDetailsView> NodePropertySlate = nullptr;


	virtual TSubclassOf<UEventflowEdGraph> GetGraphClass() const;
	virtual TSubclassOf<UEventflowEdGraphSchema> GetGraphSchemaClass() const;

	// ~ Binding
	virtual void OnGraphSelectionChanged(const FGraphPanelSelectionSet& SelectedNodes);
	virtual void OnGraphPropertyChanged(const FPropertyChangedEvent& PropertyChangedEvent);
	virtual void OnNodePropertyChanged(const FPropertyChangedEvent& PropertyChangedEvent);
	// ~ End of Binding

	UEventflowEdGraphNode* GetFirstSelectedNode(const FGraphPanelSelectionSet& SelectedNodes) const;
	virtual TArray<FName> GetTriggerGraphProperties() const;
	virtual TArray<FName> GetTriggerNodeProperties() const;

	// ~ FAssetEditorToolkit
	virtual void SaveAsset_Execute() override;
	// ~ End of FAssetEditorToolkit

};


// Module Macros
#undef REN_API

