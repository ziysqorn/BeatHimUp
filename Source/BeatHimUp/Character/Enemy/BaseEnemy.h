// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseCharacter/AICharacter.h"
#include "../../Interface/Damageable.h"
#include "../../Interface/CanCauseDamage.h"
#include "../../Interface/HaveAttributeSet.h"
#include "../../Interface/CanBeAggressive.h"
#include "../../Interface/HaveSpecialDeath.h"
#include "../../AttributeSet/AS_AICharacter.h"
#include "BaseEnemy.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API ABaseEnemy : public AAICharacter, public IDamageable, public ICanCauseDamage, public IHaveAttributeSet, public ICanBeAggressive, public IHaveSpecialDeath
{
	GENERATED_BODY()
	
public:
	ABaseEnemy();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameplayAttributeSet")
	UAS_AICharacter* CharacterAttributeSet = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage GE Subclass")
	TSubclassOf<UGameplayEffect> DamageGESubclass;

	bool HasBecomeAggressive = false;

	FTimerHandle AttackHandle;

	virtual void BeginPlay() override;

	UFUNCTION(Client, Reliable)
	void AttackTriggered();

	UFUNCTION(NetMulticast, Reliable)
	void Hurt(const float& remainHealth, const float& totalHealth, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	TSubclassOf<UGameplayEffect> GetDamageGESubclass() {
		return DamageGESubclass;
	}

	UAttributeSet* GetAttributeSet() override {
		return CharacterAttributeSet;
	}

	bool GetHasBecomeAggressive() override {
		return HasBecomeAggressive;
	}

	void SetHasBecomeAggressive(bool inHasBecomeAggress) override {
		HasBecomeAggressive = inHasBecomeAggress;
	}

	float GetDeathDelay() override {
		return 3.f;
	}

	void ExecuteAfterDeathBehaviour(AController* inInstigator, AActor* DamageCauser) override;
};
