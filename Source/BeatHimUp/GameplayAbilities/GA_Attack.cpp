// Fill out your copyright notice in the Description page of Project Settings.

#include "GA_Attack.h"

void UGA_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	GEngine->AddOnScreenDebugMessage(0, 5.0f, FColor::Blue, "This character is attacking");
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
