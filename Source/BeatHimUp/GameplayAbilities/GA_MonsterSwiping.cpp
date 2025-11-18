// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_MonsterSwiping.h"
#include "../ActorComponent/HitStopComponent/HitStopComponent.h"
#include "../Weapon/Weapon.h"
#include "../Character/BaseCharacter/BaseCharacter.h"

UGA_MonsterSwiping::UGA_MonsterSwiping()
{

}

void UGA_MonsterSwiping::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (ActorInfo) {
		CurrentSpecHandle = Handle;
		CurrentActorInfo = ActorInfo;
		CurrentActivationInfo = ActivationInfo;
		FGameplayTag targetHitTag = FGameplayTag::RequestGameplayTag(FName("GameplayEvent.TargetAttacked"));
		if (UAbilityTask_WaitGameplayEvent* WaitForTargetHitTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, targetHitTag, nullptr, true, false)) {
			WaitForTargetHitTask->EventReceived.AddDynamic(this, &UGA_MonsterSwiping::TargetHit);
			WaitForTargetHitTask->ReadyForActivation();
		}
		if (AWeapon* Weapon = Cast<AWeapon>(GetSourceObject(Handle, ActorInfo)))
		{
			UAnimMontage* Montage = Weapon->GetMontage();
			if (Montage)
			{
				if (UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("PlayAttackMontageAndWait"), Montage)) {
					Weapon->AddWeaponStateTag(FGameplayTag::RequestGameplayTag(FName("WeaponState.Attack")));
					Task->OnCompleted.AddDynamic(this, &UGA_MonsterSwiping::AttackEnd);
					Task->ReadyForActivation();
				}

			}
		}
	}
}

bool UGA_MonsterSwiping::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	return true;
}

void UGA_MonsterSwiping::AttackEnd()
{
	if (AWeapon* Weapon = Cast<AWeapon>(GetSourceObject(CurrentSpecHandle, CurrentActorInfo)))
	{
		Weapon->RemoveWeaponStateTag(FGameplayTag::RequestGameplayTag(FName("WeaponState.Attack")));
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}


void UGA_MonsterSwiping::TargetHit_Implementation(FGameplayEventData eventData)
{
	if (const ACharacter* TargetCharacter = Cast<ACharacter>(eventData.Target)) {
		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(GE_ApplyDamageSubclass);
		if (const IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(TargetCharacter)) {
			if (UAbilitySystemComponent* TargetAbilitySystemComp = AbilitySystemInterface->GetAbilitySystemComponent()) {
				if (UAbilitySystemComponent* SelfAbilitySystemComp = this->GetAbilitySystemComponentFromActorInfo()) {
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

