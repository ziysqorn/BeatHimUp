// Fill out your copyright notice in the Description page of Project Settings.

#include "GA_SwordAndShieldAttack.h"
#include "../ActorComponent/HitStopComponent/HitStopComponent.h"
#include "../Weapon/Weapon.h"
#include "../Character/BaseCharacter/BaseCharacter.h"
#include "../Interface/CanCauseDamage.h"

UGA_SwordAndShieldAttack::UGA_SwordAndShieldAttack()
{
}

void UGA_SwordAndShieldAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (ActorInfo) {
		CurrentSpecHandle = Handle;
		CurrentActorInfo = ActorInfo;
		CurrentActivationInfo = ActivationInfo;
		this->OnGameplayAbilityEnded.AddUObject(this, &UGA_SwordAndShieldAttack::AbilityEndedCallback);
		FGameplayTag targetHitTag = FGameplayTag::RequestGameplayTag(FName("GameplayEvent.TargetAttacked"));
		if (UAbilityTask_WaitGameplayEvent* WaitForTargetHitTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, targetHitTag, nullptr, false, false)) {
			WaitForTargetHitTask->EventReceived.AddDynamic(this, &UGA_SwordAndShieldAttack::TargetHit);
			WaitForTargetHitTask->ReadyForActivation();
		}
		if (AWeapon* Weapon = Cast<AWeapon>(GetSourceObject(Handle, ActorInfo)))
		{
			Weapon->ClearHandledActors(2);
		}
		if (ABaseCharacter* OwnCharacter = Cast<ABaseCharacter>(ActorInfo->OwnerActor))
		{
			if (UMontagesDataAsset* DA_Montages = OwnCharacter->GetMontagesDataAsset()) {
				if (FMontageSet* MontageSet = DA_Montages->MontagesMap.Find("Attack")) {
					if (UAnimMontage** MontageToPlay = MontageSet->ActionMontages.Find("SwordAndShield")) {
						if (MontageToPlay && *MontageToPlay)
						{
							if (UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("PlayAttackMontageAndWait"), *MontageToPlay)) {
								Task->OnCompleted.AddDynamic(this, &UGA_SwordAndShieldAttack::AttackEnd);
								Task->ReadyForActivation();
								CommitAbility(Handle, ActorInfo, ActivationInfo);
							}
						}
					}
				}
			}
		}
	}
}

bool UGA_SwordAndShieldAttack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags) && CheckCost(Handle, ActorInfo);
}


void UGA_SwordAndShieldAttack::AttackEnd()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_SwordAndShieldAttack::AbilityEndedCallback(UGameplayAbility* Ability)
{
	if (AWeapon* Weapon = Cast<AWeapon>(GetSourceObject(CurrentSpecHandle, CurrentActorInfo)))
	{
		if (Weapon->GetBoxComp()) {
			Weapon->GetBoxComp()->SetCollisionProfileName(FName("WeaponOfflinePreset"));
		}
	}
}


void UGA_SwordAndShieldAttack::TargetHit_Implementation(FGameplayEventData eventData)
{
	if (const ACharacter* TargetCharacter = Cast<ACharacter>(eventData.Target)) {
		if (const IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(TargetCharacter)) {
			if (UAbilitySystemComponent* TargetAbilitySystemComp = AbilitySystemInterface->GetAbilitySystemComponent()) {
				if (UAbilitySystemComponent* SelfAbilitySystemComp = this->GetAbilitySystemComponentFromActorInfo()) {
					if (ICanCauseDamage* CanCauseDamage = Cast<ICanCauseDamage>(this->GetOwningActorFromActorInfo())) {
						FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(CanCauseDamage->GetDamageGESubclass());
						//Send Target's Shield's Defence Stats to Gameplay Effect Calculation
						if (SpecHandle.Data) SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.ShieldDef")), eventData.EventMagnitude);
						SelfAbilitySystemComp->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetAbilitySystemComp);
						if (CurrentActorInfo) {
							if (AActor* OwnerActor = CurrentActorInfo->OwnerActor.Get()) {
								if (UHitStopComponent* HitStopComp = OwnerActor->FindComponentByClass<UHitStopComponent>()) {
									HitStopComp->NetMulticast_HitStop(HitStopDuration, HitStopDilation);
									HitStopComp->Client_ShakeCameraOnHit();
								}
							}
							if (AActor* TargetActor = TargetAbilitySystemComp->GetOwnerActor()) {
								if (UHitStopComponent* HitStopComp = TargetActor->FindComponentByClass<UHitStopComponent>()) {
									HitStopComp->NetMulticast_HitStop(HitStopDuration, HitStopDilation);
									HitStopComp->Client_ShakeCameraOnHit();
								}
							}
						}
					}
				}
			}
		}
	}
}
