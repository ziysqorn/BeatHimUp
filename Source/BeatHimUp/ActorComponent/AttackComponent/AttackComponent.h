// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttackComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BEATHIMUP_API UAttackComponent : public UActorComponent
{
	GENERATED_BODY()

private:

public:	
	// Sets default values for this component's properties
	UAttackComponent();


	void PerformAttack() {

	}
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
		
};
