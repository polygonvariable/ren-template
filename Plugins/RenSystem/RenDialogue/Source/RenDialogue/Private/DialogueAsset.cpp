// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "DialogueAsset.h"

// Engine Headers

// Project Headers



const TArray<FText>* UDialogueNodeData::GetOutputOptions() const
{
	return &OutputOptions;
}

