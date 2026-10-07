// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "ComponentDefinition.h"


/**
 *
 */
class FComponentTemplateLibrary
{

public:

    CGAMEDATACOMPONENT_API static void RegisterComponents(AActor* InActor, TArray<FComponentDefinition>& InComponents, TArray<UActorComponent*>& OutComponents);
    CGAMEDATACOMPONENT_API static void RegisterComponents(AActor* InActor, TArray<FComponentDefinition>& InComponents);

};

