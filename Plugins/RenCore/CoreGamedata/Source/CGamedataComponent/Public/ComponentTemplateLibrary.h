// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "StructUtils/InstancedStruct.h"

// Project Headers
#include "ComponentTemplateDefinition.h"


/**
 *
 */
class FComponentTemplateLibrary
{

public:

    CGAMEDATACOMPONENT_API static void RegisterComponents(AActor* InActor, TArray<TInstancedStruct<FComponentTemplateDefinition>>& InComponents, TArray<UActorComponent*>& OutComponents);
    CGAMEDATACOMPONENT_API static void RegisterComponents(AActor* InActor, TArray<TInstancedStruct<FComponentTemplateDefinition>>& InComponents);

};

