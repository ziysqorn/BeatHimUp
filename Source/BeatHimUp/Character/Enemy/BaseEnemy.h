// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseCharacter/AICharacter.h"
#include "BaseEnemy.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API ABaseEnemy : public AAICharacter
{
	GENERATED_BODY()
	
public:
	ABaseEnemy();

protected:
	virtual void BeginPlay() override;
};
