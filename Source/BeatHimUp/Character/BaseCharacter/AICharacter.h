// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "AICharacter.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API AAICharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
public:
	AAICharacter();

protected:
	virtual void BeginPlay() override;
};
