// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Util/ObjectUtil.h"

// Engine Headers
#include "StructUtils/PropertyBag.h"
#include "UObject/UnrealType.h"


void FObjectUtil::CopyPropertiesToInstance(const FInstancedPropertyBag& Properties, UObject* Object)
{
	if (!IsValid(Object))
	{
		return;
	}

	const UPropertyBag* BagStruct = Properties.GetPropertyBagStruct();
	const uint8* BagMemory = Properties.GetValue().GetMemory();

	if (BagStruct && BagMemory)
	{
		for (TFieldIterator<FProperty> BagPropertyIt(BagStruct); BagPropertyIt; ++BagPropertyIt)
		{
			FProperty* BagProperty = *BagPropertyIt;
			FName PropertyName = BagProperty->GetFName();

			UClass* ObjectClass = Object->GetClass();
			FProperty* TargetProperty = ObjectClass->FindPropertyByName(PropertyName);

			if (TargetProperty && TargetProperty->SameType(BagProperty))
			{
				const void* SourceData = BagProperty->ContainerPtrToValuePtr<void>(BagMemory);
				void* DestinationData = TargetProperty->ContainerPtrToValuePtr<void>(Object);

				TargetProperty->CopyCompleteValue(DestinationData, SourceData);
			}
		}
	}
}

void FObjectUtil::CopyPropertiesToBag(UClass* Class, FInstancedPropertyBag& OutProperties)
{
	if (IsValid(Class))
	{
		for (TFieldIterator<FProperty> It(Class); It; ++It)
		{
			FProperty* Property = *It;

			bool bIsExposedOnSpawn = Property->HasAnyPropertyFlags(CPF_ExposeOnSpawn);
			bool bHasEditFlag = Property->HasAnyPropertyFlags(CPF_Edit);
			bool bIsDisabledOnInstance = Property->HasAnyPropertyFlags(CPF_DisableEditOnInstance);
			bool bIsInstanceEditable = bHasEditFlag && !bIsDisabledOnInstance;

			if (bIsInstanceEditable && bIsExposedOnSpawn)
			{
				FString PropertyName = Property->GetName();
				OutProperties.AddProperty(FName(*PropertyName), Property);
			}
		}
	}
}

void FObjectUtil::CleanupBagProperties(UClass* Class, FInstancedPropertyBag& OutProperties)
{
	const UPropertyBag* BagStruct = OutProperties.GetPropertyBagStruct();

	if (IsValid(Class) && BagStruct)
	{
		TArray<FName> InvalidProperties;

		for (TFieldIterator<FProperty> BagPropertyIt(BagStruct); BagPropertyIt; ++BagPropertyIt)
		{
			FProperty* BagProperty = *BagPropertyIt;
			FName PropertyName = BagProperty->GetFName();

			FProperty* TargetProperty = Class->FindPropertyByName(PropertyName);
			if (!TargetProperty || !TargetProperty->SameType(BagProperty))
			{
				InvalidProperties.Add(PropertyName);
			}
		}

		OutProperties.RemovePropertiesByName(InvalidProperties);
	}
}
