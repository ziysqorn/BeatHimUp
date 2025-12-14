// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ProjectIncludes.h"
#include "../Interface/HaveSpecialDeath.h"
#include "GA_Dead.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API UGA_Dead : public UGameplayAbility
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
	TObjectPtr<AController> CauseDeadInstigator = nullptr;

	UPROPERTY()
	TObjectPtr<AActor> DamageCauser = nullptr;

	UPROPERTY()
	FTimerHandle DeadDelayHandle;

	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void DeadAnimEnd();

	UFUNCTION()
	void DeadDelayEnd();

public:
	void SetupDeadInfo(AController* EventInstigator, AActor* Causer) {
		CauseDeadInstigator = EventInstigator;
		DamageCauser = Causer;
	}
};
