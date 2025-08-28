// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Interface/AttackStrategy.h"
#include "AttackComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BEATHIMUP_API UAttackComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	IAttackStrategy* attackStrategy = nullptr;

public:	
	// Sets default values for this component's properties
	UAttackComponent();

	void SetAttackStrategy(IAttackStrategy* inStrategy) {
		attackStrategy = inStrategy;
	}

	void PerformAttack() {
		if (attackStrategy) attackStrategy->Attack();
	}
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
		
};
