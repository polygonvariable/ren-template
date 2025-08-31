// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EventflowNodeData.h"

// Engine Headers

// Project Headers
#include "RenEventflow/Public/EventflowBlueprint.h"


#if WITH_EDITOR

TArray<FString> UEventflowNodeData::GetBlueprintMethods()
{
	if (!GraphBlueprint) return {};

	TArray<FString> Methods;
	for (TFieldIterator<UFunction> FIT(GraphBlueprint, EFieldIteratorFlags::IncludeSuper); FIT; ++FIT)
	{
		UFunction* Function = *FIT;

		TArray<FFieldClass*> ExpectedTypes;
		ExpectedTypes.Add(FObjectProperty::StaticClass());

		FString FunctionName = Function->GetName();
		bool bIsNative = Function->HasAllFunctionFlags(FUNC_BlueprintEvent) && Function->HasAllFunctionFlags(FUNC_BlueprintCallable);
		bool bHasPrefix = FunctionName.StartsWith("Node_");
		bool bMatchesSignature = UEventflowBlueprint::HasValidSignature(Function, ExpectedTypes);

		if (bIsNative && bHasPrefix && bMatchesSignature)
		{
			Methods.Add(FunctionName);
		}
	}

	return Methods;
}

#endif

