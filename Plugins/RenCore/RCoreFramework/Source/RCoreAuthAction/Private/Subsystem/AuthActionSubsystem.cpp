// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Subsystem/AuthActionSubsystem.h"

// Project Headers
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Auth/AuthAction.h"


UAuthAction* UAuthActionSubsystem::AcquireFromList(TSubclassOf<UAuthAction> ActionClass)
{
	TObjectPtr<UAuthAction>* StartingNode = ActionList.Find(ActionClass);
	if (StartingNode)
	{
		UAuthAction* Action = StartingNode->Get();
		if (IsValid(Action))
		{
			ActionList.Add(ActionClass, Action->NextNode);
			Action->NextNode = nullptr;
			return Action;
		}
	}
	return NewObject<UAuthAction>(this, ActionClass);
}

void UAuthActionSubsystem::ReturnToList(UAuthAction* Action)
{
	if (!IsValid(Action))
	{
		return;
	}

	UClass* ActionClass = Action->GetClass();
	if (ActionList.Contains(ActionClass))
	{
		Action->NextNode = ActionList.FindChecked(ActionClass);
	}
	ActionList.Add(ActionClass, Action);
}


UAuthAction* UAuthActionSubsystem::CreateAction(FGuid ActionId, TSubclassOf<UAuthAction> ActionClass)
{
	if (ActiveActions.Contains(ActionId))
	{
		LOG_ERROR(LogAction, TEXT("Latent task already exists"));
		return nullptr;
	}

	UAuthAction* Action = AcquireFromList(ActionClass);
	if (IsValid(Action))
	{
		ActiveActions.Add(ActionId, Action);

		Action->SetActionId(ActionId);
		Action->OnActionFinished.BindUObject(this, &UAuthActionSubsystem::HandleOnActionFinished);
	}

	return Action;
}

void UAuthActionSubsystem::StartAction(FGuid ActionId)
{
	UAuthAction* Action = FindAction(ActionId);
	if (!IsValid(Action))
	{
		LOG_ERROR(LogAction, TEXT("Latent task is invalid"));
		return;
	}

	Action->StartAction();
}

void UAuthActionSubsystem::StopAction(FGuid ActionId)
{
	UAuthAction* Action = FindAction(ActionId);
	if (!IsValid(Action))
	{
		LOG_ERROR(LogAction, TEXT("Latent task is invalid"));
		return;
	}

	Action->StopAction();
}

UAuthAction* UAuthActionSubsystem::FindAction(FGuid ActionId)
{
	TObjectPtr<UAuthAction>* ActionItem = ActiveActions.Find(ActionId);
	if (!ActionItem)
	{
		return nullptr;
	}

	return ActionItem->Get();
}

void UAuthActionSubsystem::HandleOnActionFinished(FGuid ActionId)
{
	UAuthAction* Action = FindAction(ActionId);
	if (!IsValid(Action))
	{
		LOG_ERROR(LogAction, TEXT("Latent task is invalid"));
		return;
	}

	ActiveActions.Remove(ActionId);

	ReturnToList(Action);
}

bool UAuthActionSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UAuthActionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	LOG_WARNING(LogAction, TEXT("AuthActionSubsystem initialized"));
}

void UAuthActionSubsystem::Deinitialize()
{
	LOG_WARNING(LogAction, TEXT("AuthActionSubsystem deinitialized"));

	Super::Deinitialize();
}


UAuthActionSubsystem* UAuthActionSubsystem::Get(UWorld* World)
{
	if (!IsValid(World))
	{
		return nullptr;
	}
	return Get(World->GetGameInstance());
}

UAuthActionSubsystem* UAuthActionSubsystem::Get(UGameInstance* GameInstance)
{
	if (!IsValid(GameInstance))
	{
		return nullptr;
	}
	return GameInstance->GetSubsystem<UAuthActionSubsystem>();
}


