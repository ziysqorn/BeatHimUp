// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ProjectIncludes.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "../GameplayEffect/GE_ApplyDamage.h"
#include "../AttributeSet/AttributeSet_PlayableCharacter.h"
#include "GA_SwordAndShieldAttack.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API UGA_SwordAndShieldAttack : public UGameplayAbility
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EditorProperties | MontageToPlay")
	UAnimMontage* SwordAndShieldAttackMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EditorProperties | ApplyDamageGameplayEffect")
	TSubclassOf<UGE_ApplyDamage> GE_ApplyDamageSubclass;

	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	UFUNCTION()
	void AttackEnd();

	UFUNCTION(Server, Reliable)
	void TargetHit(FGameplayEventData eventData);
};
