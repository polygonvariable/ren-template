// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Task/EventflowTask.h"

// Project Headers
#include "EventflowEngine.h"


void UEventflowTask::CopyFromAsset(const UEventflowTask* Template)
{

}

UWorld* UEventflowTask::GetWorld() const
{
	return GetOuter()->GetWorld();
}

#if WITH_EDITOR

void UEventflowTask::AppendAssetBundleData(FAssetBundleData& InAssetBundleData)
{

}

bool UEventflowTask::ImplementsGetWorld() const
{
	return true;
}

#endif

UEventflowEngine* UEventflowTask::GetOwningEngine() const
{
	return Cast<UEventflowEngine>(GetOuter());
}

