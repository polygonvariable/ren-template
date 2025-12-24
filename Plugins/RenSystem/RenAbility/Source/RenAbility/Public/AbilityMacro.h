// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"



#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName) \

// For Bluperintable Attributes
#define SET_ATTRIBUTE(Name) \
	if(Attribute == Get##Name##Attribute()) { \
		Set##Name(NewValue); \
		return; \
	} \

#define GET_ATTRIBUTE(Name) \
	if(Attribute == Get##Name##Attribute()) { \
		return Get##Name(); \
	} \

