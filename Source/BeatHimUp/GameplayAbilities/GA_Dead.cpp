// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Dead.h"
#include "../Character/BaseCharacter/BaseCharacter.h"

void UGA_Dead::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (ActorInfo) {
		CurrentSpecHandle = Handle;
		CurrentActorInfo = ActorInfo;
		CurrentActivationInfo = ActivationInfo;
		/*if (ABaseCharacter* OwnCharacter = Cast<ABaseCharacter>(ActorInfo->OwnerActor))
		{
			if (UMontagesDataAsset* DA_Montages = OwnCharacter->GetMontagesDataAsset()) {
				if (FMontageSet* MontageSet = DA_Montages->MontagesMap.Find("State")) {
					if (UAnimMontage** MontageToPlay = MontageSet->ActionMontages.Find("Dead")) {
						if (MontageToPlay && *MontageToPlay)
						{
							if (UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("PlayDeadMontageAndWait"), *MontageToPlay)) {
								Task->ReadyForActivation();
							}
						}
					}
				}
			}
		}*/
	}
}
