// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "NativeGameplayTags.h"
#include "InstancedStruct.h"
#include "UObject/Interface.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Kismet/BlueprintAsyncActionBase.h"

#include "RCoreLibrary/Public/LogMacro.h"

// Generated Headers
#include "EffectSubsystem.generated.h"

// Forward Declarations
struct FGameplayEventData;


/*
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
class UGameplayNotifySubsystem : public UWorldSubsystem
{

	GENERATED_BODY()


public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameplayEvent, FGameplayTag, EventTag, FGameplayEventData, Payload);
	UPROPERTY(BlueprintAssignable)
	FOnGameplayEvent OnGameplayEvent;


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




	void SendGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload);


	static UGameplayNotifySubsystem* Get(const UWorld* World)
	{
		if (!IsValid(World))
		{
			return nullptr;
		}
		return World->GetSubsystem<UGameplayNotifySubsystem>();
	}

protected:

	TMap<FGameplayTag, FGameplayEventListeners> ListenerMap;

	template<typename InterfaceType, typename FuncType>
	void BroadcastEvent(const FGameplayTag& EventTag, FuncType Func);

	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;

};

template<typename InterfaceType, typename FuncType>
inline void UGameplayNotifySubsystem::BroadcastEvent(const FGameplayTag& EventTag, FuncType Func)
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
*/


/*
 * 
 * 
 * 
 */
USTRUCT(BlueprintType)
struct FGameplayNotifyPacket
{

	GENERATED_BODY()

public:

	FGameplayNotifyPacket() {};
	FGameplayNotifyPacket(float InMagnitude) : Magnitude(InMagnitude) {};
	FGameplayNotifyPacket(float InMagnitude, const FGameplayTagContainer& InTags) : Magnitude(InMagnitude), Tags(InTags) {};
	

	UPROPERTY(BlueprintReadOnly)
	float Magnitude = 0.0f;

	UPROPERTY(BlueprintReadOnly)
	FGameplayTagContainer Tags;

	UPROPERTY()
	TObjectPtr<const UObject> OptionalObject;

};


DECLARE_MULTICAST_DELEGATE_OneParam(FGameplayNotifyDelegate, const FGameplayNotifyPacket& /* Data */);

/*
 * 
 * 
 * 
 */
UCLASS()
class UGameplayNotifySubsystem : public UWorldSubsystem
{

	GENERATED_BODY()

public:

	void SendNotify(FGameplayTag NotifyTag, const FGameplayNotifyPacket& Data, bool bReliable);
	void BroadcastNotify(FGameplayTag NotifyTag, const FGameplayNotifyPacket& Data);

	FGameplayNotifyDelegate& GetNotifyDelegate(FGameplayTag NotifyTag);
	FGameplayNotifyDelegate* FindNotifyDelegate(FGameplayTag NotifyTag);

	// ~ UWorldSubsystem
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;
	virtual void Deinitialize() override;
	// ~ End of UWorldSubsystem

protected:

	TMap<FGameplayTag, FGameplayNotifyDelegate> RegisteredNotifies;

	UPROPERTY()
	TObjectPtr<AGameplayNotifyNetworkActor> NetworkActor;

};



/*
 * Actor to manage event replication send by the UGameplayNotifySubsystem.
 * 
 * 
 */
UCLASS()
class AGameplayNotifyNetworkActor : public AInfo
{

	GENERATED_BODY()

public:

	AGameplayNotifyNetworkActor();

	void SendNotify(FGameplayTag NotifyTag, const FGameplayNotifyPacket& Data, bool bReliable);

	// ~ AInfo
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// ~ End of AInfo

protected:

	UPROPERTY()
	TWeakObjectPtr<UGameplayNotifySubsystem> NotifySubsystem;


	UFUNCTION(NetMulticast, Reliable)
	void SendNotifyReliable(FGameplayTag NotifyTag, const FGameplayNotifyPacket& Data);
	virtual void SendNotifyReliable_Implementation(FGameplayTag NotifyTag, const FGameplayNotifyPacket& Data);

	UFUNCTION(NetMulticast, Unreliable)
	void SendNotifyUnreliable(FGameplayTag NotifyTag, const FGameplayNotifyPacket& Data);
	virtual void SendNotifyUnreliable_Implementation(FGameplayTag NotifyTag, const FGameplayNotifyPacket& Data);

	void SendNotifyInternal(FGameplayTag NotifyTag, const FGameplayNotifyPacket& Data);

	// ~ AInfo
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	// ~ End of AInfo

};



/**
 * 
 * 
 * 
 */
UCLASS(BlueprintType, Meta = (ExposedAsyncProxy = AsyncTask))
class UWaitForGameplayNotify : public UBlueprintAsyncActionBase
{

	GENERATED_BODY()

public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNotifyReceived, FGameplayNotifyPacket, Data);
	UPROPERTY(BlueprintAssignable)
	FOnNotifyReceived OnReceived;


	UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = "true", WorldContext = "InWorldContext"))
	static UWaitForGameplayNotify* WaitForGameplayNotify(FGameplayTag InNotifyTag, UObject* InWorldContext);

	UFUNCTION(BlueprintCallable)
	void EndTask();

protected:

	FGameplayTag NotifyTag;

	UPROPERTY()
	TWeakObjectPtr<UObject> WorldContext;


	UFUNCTION()
	void HandleNotifyReceived(const FGameplayNotifyPacket& Data);

	// ~ UBlueprintAsyncActionBase
	virtual void Activate() override;
	// ~ End of UBlueprintAsyncActionBase

};

