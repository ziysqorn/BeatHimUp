// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ProjectIncludes.h"

#include "HaveSpecialDeath.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHaveSpecialDeath : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BEATHIMUP_API IHaveSpecialDeath
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual float GetDeathDelay() = 0;
	virtual void ExecuteAfterDeathBehaviour(AController * inInstigator, AActor* DamageCauser) = 0;
};
