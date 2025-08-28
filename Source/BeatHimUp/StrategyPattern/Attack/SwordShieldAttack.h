// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../../Interface/AttackStrategy.h"
#include "SwordShieldAttack.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API USwordShieldAttack : public UObject, public IAttackStrategy
{
	GENERATED_BODY()
	
public:
	USwordShieldAttack() {

	}

	void Attack() override {
		GEngine->AddOnScreenDebugMessage(0, 5.0f, FColor::Red, "Sword And Shield Attack");
	}
};
