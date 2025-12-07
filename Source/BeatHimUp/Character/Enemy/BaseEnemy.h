// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseCharacter/AICharacter.h"
#include "../../Interface/Damageable.h"
#include "../../Interface/CanCauseDamage.h"
#include "../../AttributeSet/AS_AICharacter.h"
#include "BaseEnemy.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API ABaseEnemy : public AAICharacter, public IDamageable, public ICanCauseDamage
{
	GENERATED_BODY()
	
public:
	ABaseEnemy();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameplayAttributeSet")
	UAS_AICharacter* CharacterAttributeSet = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage GE Subclass")
	TSubclassOf<UGameplayEffect> DamageGESubclass;

	FTimerHandle AttackHandle;

	virtual void BeginPlay() override;

	UFUNCTION(Client, Reliable)
	void AttackTriggered();

	UFUNCTION(NetMulticast, Reliable)
	void Hurt(const float& remainHealth, const float& totalHealth, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	TSubclassOf<UGameplayEffect> GetDamageGESubclass() {
		return DamageGESubclass;
	}
};
