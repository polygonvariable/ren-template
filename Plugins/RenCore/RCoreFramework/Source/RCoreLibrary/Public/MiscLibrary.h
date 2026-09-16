// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Module Macros
#define REN_API RCORELIBRARY_API


/*
 *
 */
namespace FMiscLibrary
{

	REN_API float PackFloats(float ValueA, float ValueB);
	REN_API UWorld* GetCurrentWorld();

};


// Module Macros
#undef REN_API

