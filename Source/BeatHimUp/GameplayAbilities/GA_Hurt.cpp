// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Hurt.h"
#include "../Character/BaseCharacter/BaseCharacter.h"

void UGA_Hurt::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (ActorInfo) {
		CurrentSpecHandle = Handle;
		CurrentActorInfo = ActorInfo;
		CurrentActivationInfo = ActivationInfo;
		if (ABaseCharacter* OwnCharacter = Cast<ABaseCharacter>(ActorInfo->OwnerActor))
		{
			if (UMontagesDataAsset* DA_Montages = OwnCharacter->GetMontagesDataAsset()) {
				if (FMontageSet* MontageSet = DA_Montages->MontagesMap.Find("State")) {
					if (UAnimMontage** MontageToPlay = MontageSet->ActionMontages.Find("Hurt")) {
						if (MontageToPlay && *MontageToPlay)
						{
							if (UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("PlayHurtMontageAndWait"), *MontageToPlay)) {
								Task->OnCompleted.AddDynamic(this, &UGA_Hurt::HurtEnd);
								Task->ReadyForActivation();
							}
						}
					}
				}
			}
		}
	}
}

void UGA_Hurt::HurtEnd()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
