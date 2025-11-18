// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Block.h"

void UGA_Block::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) {
	if (ActorInfo) {
		CurrentSpecHandle = Handle;
		CurrentActorInfo = ActorInfo;
		CurrentActivationInfo = ActivationInfo;
		FGameplayTag targetHitTag = FGameplayTag::RequestGameplayTag(FName("GameplayEvent.Parried"));
		if (UAbilityTask_WaitGameplayEvent* WaitForParriedTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, targetHitTag, nullptr, true, false)) {
			WaitForParriedTask->EventReceived.AddDynamic(this, &UGA_Block::AttackParried);
			WaitForParriedTask->ReadyForActivation();
		}
		CommitAbility(Handle, ActorInfo, ActivationInfo);
	}
}

void UGA_Block::AttackParried(FGameplayEventData eventData) {
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Parried"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}



