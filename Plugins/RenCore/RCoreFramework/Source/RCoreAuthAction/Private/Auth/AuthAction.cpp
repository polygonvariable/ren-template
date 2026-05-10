// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Auth/AuthAction.h"

// Project Headers
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"


bool UAuthAction::StartAction()
{
	if (!ActionId.IsValid())
	{
		return false;
	}

	LOG_WARNING(LogAction, TEXT("Auth action started"));

	OnStarted();
	return true;
}

void UAuthAction::StopAction()
{
	LOG_WARNING(LogAction, TEXT("Auth action stopped"));

	OnCompleted(false);
	OnActionFinished.ExecuteIfBound(ActionId);
	Cleanup();
}

void UAuthAction::OnStarted()
{

}

void UAuthAction::OnCompleted(bool bSuccess)
{

}

void UAuthAction::OnCleanup()
{

}

void UAuthAction::Success()
{
	LOG_INFO(LogAction, TEXT("Auth action completed"));

	OnCompleted(true);
	OnActionFinished.ExecuteIfBound(ActionId);
	Cleanup();
}

void UAuthAction::Fail(const FString& Reason)
{
	LOG_ERROR(LogAction, TEXT("%s"), *Reason);

	OnCompleted(false);
	OnActionFinished.ExecuteIfBound(ActionId);
	Cleanup();
}

void UAuthAction::Cleanup()
{
	OnCleanup();

	OnActionFinished.Unbind();
	ActionId.Invalidate();
}

FGuid UAuthAction::GetActionId()
{
	return ActionId;
}

void UAuthAction::SetActionId(FGuid NewId)
{
	if (!ActionId.IsValid())
	{
		ActionId = NewId;
	}
}

