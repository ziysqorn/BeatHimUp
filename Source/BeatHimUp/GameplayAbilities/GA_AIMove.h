// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ProjectIncludes.h"
#include "Abilities/Tasks/AbilityTask_MoveToLocation.h"
#include "GA_AIMove.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API UGA_AIMove : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
