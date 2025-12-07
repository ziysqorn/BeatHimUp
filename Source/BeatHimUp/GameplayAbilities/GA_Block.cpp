// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Block.h"
#include "../Weapon/Weapon.h"
#include "../Character/BaseCharacter/BaseCharacter.h"

UGA_Block::UGA_Block()
{
}

void UGA_Block::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) {
	if (ActorInfo) {
		CurrentSpecHandle = Handle;
		CurrentActorInfo = ActorInfo;
		CurrentActivationInfo = ActivationInfo;
		this->OnGameplayAbilityEnded.AddUObject(this, &UGA_Block::AbilityEndedCallback);
		FGameplayTag parriedTag = FGameplayTag::RequestGameplayTag(FName("GameplayEvent.Parried"));
		FGameplayTag blockedTag = FGameplayTag::RequestGameplayTag(FName("GameplayEvent.Blocked"));
		if (UAbilityTask_WaitGameplayEvent* WaitForParriedTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, parriedTag, nullptr, true, false)) {
			WaitForParriedTask->EventReceived.AddDynamic(this, &UGA_Block::AttackParried);
			WaitForParriedTask->ReadyForActivation();
		}
		if (UAbilityTask_WaitGameplayEvent* WaitForBlockedTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, blockedTag, nullptr, false, false)) {
			WaitForBlockedTask->EventReceived.AddDynamic(this, &UGA_Block::AttackBlocked);
			WaitForBlockedTask->ReadyForActivation();
		}
		CommitAbility(Handle, ActorInfo, ActivationInfo);
	}
}

void UGA_Block::AttackParried(FGameplayEventData eventData) {
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Parried"));

	PlayBlockedMontage();
	//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_Block::AttackBlocked(FGameplayEventData eventData)
{
	PlayBlockedMontage();
}


void UGA_Block::PlayBlockedMontage_Implementation()
{
	if (CurrentActorInfo) {
		if (ABaseCharacter* OwnCharacter = Cast<ABaseCharacter>(CurrentActorInfo->OwnerActor))
		{
			if (UMontagesDataAsset* DA_Montages = OwnCharacter->GetMontagesDataAsset()) {
				if (FMontageSet* MontageSet = DA_Montages->MontagesMap.Find("State")) {
					if (UAnimMontage** MontageToPlay = MontageSet->ActionMontages.Find("Blocked")) {
						if (MontageToPlay && *MontageToPlay)
						{
							if (UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("PlayMontageAndWait"), *MontageToPlay)) {
								Task->ReadyForActivation();
							}

						}
					}
				}
			}
		}
	}
}

void UGA_Block::AbilityEndedCallback(UGameplayAbility* Ability)
{
	if (CurrentActorInfo) {
		if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(CurrentActorInfo->OwnerActor)) {
			if (UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent()) {
				ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Parrying")));
			}
		}
		if (AWeapon* Weapon = Cast<AWeapon>(GetSourceObject(CurrentSpecHandle, CurrentActorInfo)))
		{
			if (Weapon->GetBoxComp()) {
				Weapon->GetBoxComp()->SetCollisionProfileName(FName("WeaponOfflinePreset"));
			}
		}
	}
}


