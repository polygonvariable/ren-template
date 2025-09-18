// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers

// Generated Headers

// Forward Declarations
class UEventflowNode;

// Delegates Declarations
DECLARE_DELEGATE_OneParam(FOnNodeReached, UEventflowNode*);
DECLARE_DELEGATE_ThreeParams(FOnNodeExited, UEventflowNode* /* Node */, bool /* bSuccess */, int /* NextNodeIndex */);
DECLARE_DELEGATE(FOnGraphStarted);
DECLARE_DELEGATE(FOnGraphEnded);

