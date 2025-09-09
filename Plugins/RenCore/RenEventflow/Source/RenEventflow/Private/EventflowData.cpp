// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EventflowData.h"

// Engine Headers

// Project Headers



const UEventflowNode* UEventflowData::GetEntryNode() const
{
    const TObjectPtr<UEventflowNode>* FoundNode = NodeList.Find(NodeEntry);
    if (FoundNode)
    {
        return nullptr;
    }
    return FoundNode->Get();
}

