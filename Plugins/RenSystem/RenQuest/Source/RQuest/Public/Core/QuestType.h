// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Generated Headers
#include "QuestType.generated.h"

/**
 *
 */
UENUM()
enum class EQuestType : uint8
{
	World UMETA(DisplayName = "World"),
	Story UMETA(DisplayName = "Story"),
	Challenge UMETA(DisplayName = "Challenge"),
	//Other UMETA(DisplayName = "Other"),
};

