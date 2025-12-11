// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ProjectIncludes.h"

#include "CanBeAggressive.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCanBeAggressive : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BEATHIMUP_API ICanBeAggressive
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual bool GetHasBecomeAggressive() = 0;

	virtual void SetHasBecomeAggressive(bool inHasBecomeAggress) = 0;
};
