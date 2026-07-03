// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


/**
 *
 */
UENUM()
enum class EFSMState : uint8
{
	Uninitialized UMETA(DisplayName = "Uninitialized"),
	Initialized UMETA(DisplayName = "Initialized"),
	Loaded UMETA(DisplayName = "Loaded"),
	Ready UMETA(DisplayName = "Ready"),
	Active UMETA(DisplayName = "Active"),
	Finished UMETA(DisplayName = "Finished"),
};

/**
 *
 */
UENUM()
enum class EFSMResult : uint8
{
	None UMETA(DisplayName = "None"),
	Success UMETA(DisplayName = "Success"),
	Failed UMETA(DisplayName = "Failed"),
	Aborted UMETA(DisplayName = "Aborted"), // forced closed internally
	Cancelled UMETA(DisplayName = "Cancelled"), // cancelled by user
};

/**
 *
 */
struct FFSMTransition
{
	EFSMState State = EFSMState::Uninitialized;
	EFSMResult Result = EFSMResult::None;
};

