// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ProjectIncludes.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GA_Block.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API UGA_Block : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_Block();

protected:
	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void AttackParried(FGameplayEventData eventData);

	UFUNCTION()
	void AttackBlocked(FGameplayEventData eventData);

	UFUNCTION(NetMulticast, Unreliable)
	void PlayBlockedMontage();

	UFUNCTION()
	void AbilityEndedCallback(UGameplayAbility* Ability);
};
