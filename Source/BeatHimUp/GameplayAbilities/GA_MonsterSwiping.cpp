// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_MonsterSwiping.h"
#include "../ActorComponent/HitStopComponent/HitStopComponent.h"
#include "../Weapon/Weapon.h"
#include "../Character/BaseCharacter/BaseCharacter.h"
#include "../Interface/CanCauseDamage.h"

UGA_MonsterSwiping::UGA_MonsterSwiping()
{
}

void UGA_MonsterSwiping::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (ActorInfo) {
		CurrentSpecHandle = Handle;
		CurrentActorInfo = ActorInfo;
		CurrentActivationInfo = ActivationInfo;
		this->OnGameplayAbilityEnded.AddUObject(this, &UGA_MonsterSwiping::AbilityEndedCallback);
		FGameplayTag targetHitTag = FGameplayTag::RequestGameplayTag(FName("GameplayEvent.TargetAttacked"));
		FGameplayTag attackParriedTag = FGameplayTag::RequestGameplayTag(FName("GameplayEvent.Parried"));
		if (UAbilityTask_WaitGameplayEvent* WaitForTargetHitTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, targetHitTag, nullptr, false, false)) {
			WaitForTargetHitTask->EventReceived.AddDynamic(this, &UGA_MonsterSwiping::TargetHit);
			WaitForTargetHitTask->ReadyForActivation();
		}
		if (UAbilityTask_WaitGameplayEvent* WaitAttackParriedTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, attackParriedTag, nullptr, true, false)) {
			WaitAttackParriedTask->EventReceived.AddDynamic(this, &UGA_MonsterSwiping::AttackParried);
			WaitAttackParriedTask->ReadyForActivation();
		}
		if (AWeapon* Weapon = Cast<AWeapon>(GetSourceObject(Handle, ActorInfo)))
		{
			Weapon->ClearHandledActors(10);
		}
		if (ABaseCharacter* OwnCharacter = Cast<ABaseCharacter>(ActorInfo->OwnerActor))
		{
			if (UMontagesDataAsset* DA_Montages = OwnCharacter->GetMontagesDataAsset()) {
				if (FMontageSet* MontageSet = DA_Montages->MontagesMap.Find("Attack")) {
					if (UAnimMontage** MontageToPlay = MontageSet->ActionMontages.Find("MonsterWiping")) {
						if (MontageToPlay && *MontageToPlay)
						{
							if (UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("PlayAttackMontageAndWait"), *MontageToPlay)) {
								Task->OnCompleted.AddDynamic(this, &UGA_MonsterSwiping::AttackEnd);
								Task->ReadyForActivation();
							}

						}
					}
				}
			}
		}
	}
}

bool UGA_MonsterSwiping::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UGA_MonsterSwiping::AttackEnd()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_MonsterSwiping::AttackParried(FGameplayEventData eventData)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("This attack has been parried"));
	if (CurrentActorInfo) {
		if (UAbilitySystemComponent* ASC = CurrentActorInfo->AbilitySystemComponent.Get()) {
			FGameplayTagContainer Container;
			Container.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Hurt")));
			ASC->TryActivateAbilitiesByTag(Container);
		}
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGA_MonsterSwiping::AbilityEndedCallback(UGameplayAbility* Ability)
{
	if (AWeapon* Weapon = Cast<AWeapon>(GetSourceObject(CurrentSpecHandle, CurrentActorInfo)))
	{
		if (Weapon->GetBoxComp()) {
			Weapon->GetBoxComp()->SetCollisionProfileName(FName("WeaponOfflinePreset"));
		}
	}
}


void UGA_MonsterSwiping::TargetHit_Implementation(FGameplayEventData eventData)
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

