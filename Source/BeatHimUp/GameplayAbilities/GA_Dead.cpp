// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Dead.h"
#include "../Character/BaseCharacter/BaseCharacter.h"

void UGA_Dead::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (ActorInfo) {
		CurrentSpecHandle = Handle;
		CurrentActorInfo = ActorInfo;
		CurrentActivationInfo = ActivationInfo;
		if (ABaseCharacter* OwnCharacter = Cast<ABaseCharacter>(ActorInfo->OwnerActor))
		{
			if (UMontagesDataAsset* DA_Montages = OwnCharacter->GetMontagesDataAsset()) {
				if (FMontageSet* MontageSet = DA_Montages->MontagesMap.Find("State")) {
					if (UAnimMontage** MontageToPlay = MontageSet->ActionMontages.Find("Dead")) {
						if (MontageToPlay && *MontageToPlay)
						{
							if (UAbilityTask_WaitDelay* Task = UAbilityTask_WaitDelay::WaitDelay(this, (*MontageToPlay)->GetPlayLength()))
							{
								Task->OnFinish.AddDynamic(this, &UGA_Dead::DeadAnimEnd);
								Task->ReadyForActivation();
							}
						}
					}
				}
			}
		}
	}
}

void UGA_Dead::DeadAnimEnd()
{
	if (CurrentActorInfo) {
		if (IHaveSpecialDeath* HaveSpecialDeath = Cast<IHaveSpecialDeath>(CurrentActorInfo->OwnerActor)) {
			//GetWorld()->GetTimerManager().SetTimer(DeadDelayHandle, FTimerDelegate::CreateUObject(this, &UGA_Dead::DeadDelayEnd), HaveSpecialDeath->GetDeathDelay(), false);
			if (UAbilityTask_WaitDelay* Task = UAbilityTask_WaitDelay::WaitDelay(this, HaveSpecialDeath->GetDeathDelay()))
			{
				Task->OnFinish.AddDynamic(this, &UGA_Dead::DeadDelayEnd);
				Task->ReadyForActivation();
			}
		}
	}
}

void UGA_Dead::DeadDelayEnd()
{
	if (CurrentActorInfo) {
		if (IHaveSpecialDeath* HaveSpecialDeath = Cast<IHaveSpecialDeath>(CurrentActorInfo->OwnerActor)) {
			HaveSpecialDeath->ExecuteAfterDeathBehaviour(CauseDeadInstigator, DamageCauser);
		}
	}
}
