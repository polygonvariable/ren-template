// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Library/TagLibrary.h"

// Engine Headers

// Project Headers
#include "MovementTag.h"



FGameplayTagContainer UTagLibrary::ConvertMovementModeToTag(EMovementMode MovementMode)
{
	FGameplayTagContainer Result;

	switch(MovementMode)
	{
		case EMovementMode::MOVE_None:
			Result.AddTag(TAG_Movement_Mode_None);
			break;
		case EMovementMode::MOVE_Walking:
			Result.AddTag(TAG_Movement_Mode_Walking);
			break;
		case EMovementMode::MOVE_NavWalking:
			Result.AddTag(TAG_Movement_Mode_NavWalking);
			break;
		case EMovementMode::MOVE_Falling:
			Result.AddTag(TAG_Movement_Mode_Falling);
			break;
		case EMovementMode::MOVE_Swimming:
			Result.AddTag(TAG_Movement_Mode_Swimming);
			break;
		case EMovementMode::MOVE_Flying:
			Result.AddTag(TAG_Movement_Mode_Flying);
			break;
		case EMovementMode::MOVE_Custom:
			Result.AddTag(TAG_Movement_Mode_Custom);
			break;
		default:
			Result.AddTag(TAG_Movement_Mode_None);
			break;
	}

	return Result;
}
