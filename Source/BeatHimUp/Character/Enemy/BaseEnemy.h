// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseCharacter/AICharacter.h"
#include "../../Interface/Damageable.h"
#include "../../AttributeSet/AS_AICharacter.h"
#include "BaseEnemy.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API ABaseEnemy : public AAICharacter, public IDamageable
{
	GENERATED_BODY()
	
public:
	ABaseEnemy();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameplayAttributeSet")
	UAS_AICharacter* CharacterAttributeSet = nullptr;

	FTimerHandle AttackHandle;

	virtual void BeginPlay() override;

	UFUNCTION(Client, Reliable)
	void AttackTriggered();

	UFUNCTION(NetMulticast, Reliable)
	void Hurt(const float& remainHealth, const float& totalHealth) override;
};
