// Fill out your copyright notice in the Description page of Project Settings.

#include "GA_SwordAndShieldAttack.h"

void UGA_SwordAndShieldAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (ActorInfo) {
		FGameplayTag targetHitTag = FGameplayTag::RequestGameplayTag(FName("GameplayEvent.TargetAttacked"));
		if (UAbilityTask_WaitGameplayEvent* WaitForTargetHitTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, targetHitTag, nullptr, true, false)) {
			WaitForTargetHitTask->EventReceived.AddDynamic(this, &UGA_SwordAndShieldAttack::TargetHit);
			WaitForTargetHitTask->ReadyForActivation();
		}
		if (IsValid(SwordAndShieldAttackMontage)) {
			if (UAbilityTask_PlayMontageAndWait* PlayerAttackMontageAndWaitTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("PlayAttackMontageAndWait"), SwordAndShieldAttackMontage)) {
				CurrentSpecHandle = Handle;
				CurrentActorInfo = ActorInfo;
				CurrentActivationInfo = ActivationInfo;
				PlayerAttackMontageAndWaitTask->OnCompleted.AddDynamic(this, &UGA_SwordAndShieldAttack::AttackEnd);
				PlayerAttackMontageAndWaitTask->ReadyForActivation();
				CommitAbility(Handle, ActorInfo, ActivationInfo);
			}
		}
	}
}

bool UGA_SwordAndShieldAttack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	/*if (ActorInfo) {
		if (ActorInfo->AbilitySystemComponent.IsValid()) {
			if (const UAttributeSet_PlayableCharacter* AS_PlayerableChar = Cast<UAttributeSet_PlayableCharacter>(ActorInfo->AbilitySystemComponent->GetSet<UAttributeSet_PlayableCharacter>())) {
			}
		}
	}*/
	return CheckCost(Handle, ActorInfo);;
}

void UGA_SwordAndShieldAttack::AttackEnd()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_SwordAndShieldAttack::TargetHit_Implementation(FGameplayEventData eventData)
{
	if (const ACharacter* TargetCharacter = Cast<ACharacter>(eventData.Target)) {
		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(GE_ApplyDamageSubclass);
		if (const IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(TargetCharacter)) {
			if (UAbilitySystemComponent* TargetAbilitySystemComp = AbilitySystemInterface->GetAbilitySystemComponent()) {
				if (UAbilitySystemComponent* SelfAbilitySystemComp = this->GetAbilitySystemComponentFromActorInfo()) {
					SelfAbilitySystemComp->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetAbilitySystemComp);
				}
			}
		}
	}
}
