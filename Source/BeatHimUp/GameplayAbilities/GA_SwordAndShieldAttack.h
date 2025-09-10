// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ProjectIncludes.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GA_SwordAndShieldAttack.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API UGA_SwordAndShieldAttack : public UGameplayAbility
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EditorProperties | MontageToPlay")
	UAnimMontage* SwordAndShieldAttackMontage = nullptr;

	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void AttackEnd();

	UFUNCTION()
	void TargetHit(FGameplayEventData eventData);
};
