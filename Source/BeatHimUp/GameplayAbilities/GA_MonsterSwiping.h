// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ProjectIncludes.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GA_MonsterSwiping.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API UGA_MonsterSwiping : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_MonsterSwiping();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EditorProperties | ApplyDamageGameplayEffect")
	TSubclassOf<UGameplayEffect> GE_ApplyDamageSubclass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EditorProperties | HitStopDuration")
	float HitStopDuration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EditorProperties | HitStopDilation")
	float HitStopDilation;

	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	UFUNCTION()
	void AttackEnd();

	UFUNCTION(Server, Reliable)
	void TargetHit(FGameplayEventData eventData);
};
