// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers

// Generated Headers

// Forward Declarations



#define EVENTFLOW_NODE_DECLARE_TYPE(NodeType) \
public: \
	static FName StaticNodeType() \
	{ \
		return FName(TEXT(NodeType)); \
	} \
protected: \
	virtual FName GetStaticNodeTypeInternal() const override \
	{ \
		return StaticNodeType(); \
	}

#define EVENTFLOW_NODE_DECLARE_TITLE(NodeTitle) \
public: \
	static FText StaticNodeTitle() \
	{ \
		return FText::FromString(TEXT(NodeTitle)); \
	} \
protected: \
	virtual FText GetStaticNodeTitleInternal() const override \
	{ \
		return StaticNodeTitle(); \
	}

#define EVENTFLOW_NODE_DECLARE_DESCRIPTION(NodeDescription) \
public: \
	static FText StaticNodeDescription() \
	{ \
		return FText::FromString(TEXT(NodeDescription)); \
	} \
protected: \
	virtual FText GetStaticNodeDescriptionInternal() const override \
	{ \
		return StaticNodeDescription(); \
	}
