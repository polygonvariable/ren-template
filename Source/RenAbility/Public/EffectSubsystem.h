// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "NativeGameplayTags.h"
#include "InstancedStruct.h"
#include "UObject/Interface.h"

#include "RenCoreLibrary/Public/LogMacro.h"

// Generated Headers
#include "EffectSubsystem.generated.h"

// Forward Declarations



UINTERFACE(MinimalAPI, Blueprintable)
class UGameplayEventInterface : public UInterface
{
	GENERATED_BODY()
};

class IGameplayEventInterface
{

	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent)
	void OnReceiveEvent(const FGameplayTag& EventTag, const FInstancedStruct& Payload);

};


UINTERFACE(MinimalAPI, Blueprintable)
class UGameplayEventExtendedInterface : public UGameplayEventInterface
{
	GENERATED_BODY()
};

class IGameplayEventExtendedInterface : public IGameplayEventInterface
{

	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent)
	void OnReceiveScalarEvent(const FGameplayTag& EventTag, const float& Payload);

	UFUNCTION(BlueprintNativeEvent)
	void OnReceiveTextEvent(const FGameplayTag& EventTag, const FText& Payload);

};


USTRUCT()
struct FGameplayEventListeners
{

	GENERATED_BODY()

public:

	TSet<TWeakObjectPtr<UObject>> Listeners;

};


UCLASS()
class UGameplayEventSubsystem : public UWorldSubsystem
{

	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Meta = (DefaultToSelf = "Listener"))
	void RegisterListener(FGameplayTag EventTag, UObject* Listener);

	UFUNCTION(BlueprintCallable, Meta = (DefaultToSelf = "Listener"))
	void UnregisterListener(FGameplayTag EventTag, UObject* Listener);



	UFUNCTION(BlueprintCallable)
	void SendEvent(const FGameplayTag& EventTag, const FInstancedStruct& Payload);

	UFUNCTION(BlueprintCallable)
	void SendScalarEvent(const FGameplayTag& EventTag, const float& Payload);

	UFUNCTION(BlueprintCallable)
	void SendTextEvent(const FGameplayTag& EventTag, const FText& Payload);

protected:

	TMap<FGameplayTag, FGameplayEventListeners> ListenerMap;

	template<typename InterfaceType, typename FuncType>
	void BroadcastEvent(const FGameplayTag& EventTag, FuncType Func);

	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;

};

template<typename InterfaceType, typename FuncType>
inline void UGameplayEventSubsystem::BroadcastEvent(const FGameplayTag& EventTag, FuncType Func)
{
	if (FGameplayEventListeners* EventListeners = ListenerMap.Find(EventTag))
	{
		for (TWeakObjectPtr<UObject> Listener : EventListeners->Listeners)
		{
			if (Listener.IsValid() && Listener->Implements<InterfaceType>())
			{
				Func(Listener.Get());
			}
		}
	}
}


UCLASS(Abstract, Blueprintable, meta = (ShowWorldContextPin))
class UGameplayEventHandler : public UObject
{

	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent)
	void OnReceiveEvent(const FGameplayTag& EventTag, const FInstancedStruct& Payload, AActor* Source, AActor* Target);

	UFUNCTION(BlueprintImplementableEvent)
	void OnReceiveScalarEvent(const FGameplayTag& EventTag, const float& Payload, AActor* Source, AActor* Target);

	UFUNCTION(BlueprintImplementableEvent)
	void OnReceiveTextEvent(const FGameplayTag& EventTag, const FText& Payload, AActor* Source, AActor* Target);

	virtual UWorld* GetWorld() const override;

};


USTRUCT(BlueprintType)
struct FGameplayEventHandlers
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly)
	TSet<TObjectPtr<UGameplayEventHandler>> Handlers;

};


UCLASS(Blueprintable)
class UGameplayEventHandlerSubsystem : public UWorldSubsystem
{

	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void RegisterHandler(const FGameplayTag& EventTag, TSubclassOf<UGameplayEventHandler> HandlerClass);



	UFUNCTION(BlueprintCallable)
	void SendEvent(const FGameplayTag& EventTag, const FInstancedStruct& Payload, AActor* Source, AActor* Target);

	UFUNCTION(BlueprintCallable)
	void SendScalarEvent(const FGameplayTag& EventTag, const float& Payload, AActor* Source, AActor* Target);

	UFUNCTION(BlueprintCallable)
	void SendTextEvent(const FGameplayTag& EventTag, const FText& Payload, AActor* Source, AActor* Target);

protected:

	UPROPERTY(BlueprintReadOnly)
	TMap<FGameplayTag, FGameplayEventHandlers> HandlerMap;

	template<typename FuncType>
	void BroadcastEvent(const FGameplayTag& EventTag, AActor* Source, AActor* Target, FuncType Func);

	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;

};

template<typename FuncType>
inline void UGameplayEventHandlerSubsystem::BroadcastEvent(const FGameplayTag& EventTag, AActor* Source, AActor* Target, FuncType Func)
{
	if (FGameplayEventHandlers* EventHandlers = HandlerMap.Find(EventTag))
	{
		for (UGameplayEventHandler* Handler : EventHandlers->Handlers)
		{
			if (IsValid(Handler))
			{
				PRINT_ERROR(LogTemp, 5.0f, TEXT("Handler Name: %s"), *Handler->GetName());
				Func(Handler);
			}
			else
			{
				PRINT_ERROR(LogTemp, 5.0f, TEXT("Invalid Handler"));
			}
		}
	}
}
