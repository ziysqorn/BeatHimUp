// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Dodge.h"

void UGA_Dodge::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (IsValid(DodgeMontage)) {
		if (UAbilityTask_PlayMontageAndWait* PlayDodgeMontageAndWait = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("PlayDodgeMontageAndWait"), DodgeMontage)) {
			CurrentSpecHandle = Handle;
			CurrentActorInfo = ActorInfo;
			CurrentActivationInfo = ActivationInfo;
			PlayDodgeMontageAndWait->OnCompleted.AddDynamic(this, &UGA_Dodge::DodgeEnd);
			PlayDodgeMontageAndWait->ReadyForActivation();
			//CommitAbility(Handle, ActorInfo, ActivationInfo);
		}
	}
}

void UGA_Dodge::DodgeEnd()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

bool UGA_Dodge::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	return CheckCost(Handle, ActorInfo);
}
