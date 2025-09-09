// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers

// Generated Headers

// Forward Declarations
class UQuestGraph;



DECLARE_DELEGATE(FOnQuestObjectiveCompleted);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnQuestStarted, UQuestGraph*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnQuestResumed, UQuestGraph*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnQuestCompleted, UQuestGraph*);