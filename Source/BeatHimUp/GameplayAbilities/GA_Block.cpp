// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Block.h"
#include "../Weapon/Weapon.h"

UGA_Block::UGA_Block()
{
}

void UGA_Block::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) {
	if (ActorInfo) {
		CurrentSpecHandle = Handle;
		CurrentActorInfo = ActorInfo;
		CurrentActivationInfo = ActivationInfo;
		this->OnGameplayAbilityEnded.AddUObject(this, &UGA_Block::AbilityEndedCallback);
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
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}


void UGA_Block::AbilityEndedCallback(UGameplayAbility* Ability) 
{
	if (CurrentActorInfo) {
		if (AWeapon* Weapon = Cast<AWeapon>(GetSourceObject(CurrentSpecHandle, CurrentActorInfo)))
		{
			if (Weapon->GetBoxComp()) {
				Weapon->GetBoxComp()->SetCollisionProfileName(FName("WeaponOfflinePreset"));
			}
		}
		if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(CurrentActorInfo->OwnerActor)) {
			if (UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent()) {
				ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Parrying")));
			}
		}
	}
}


