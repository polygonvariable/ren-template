// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


enum class ETaskState : uint8
{
    None,
    Pending,
    Completed,
    Failed,
    Cancelled,
};

struct FTaskResult
{

    FTaskResult() {};
    FTaskResult(ETaskState InState) : State(InState) {};
    FTaskResult(ETaskState InState, const FString& InMessage) : State(InState), Message(InMessage) {};

    ETaskState State = ETaskState::None;
    FString Message = TEXT_EMPTY;

};

DECLARE_DELEGATE_OneParam(FOnTaskFinished, FGuid);
DECLARE_DELEGATE_OneParam(FTaskCallback, const FTaskResult&);

// DECLARE_MULTICAST_DELEGATE_TwoParams(FAuthActionCallback, FGuid, const FTaskResult&);
