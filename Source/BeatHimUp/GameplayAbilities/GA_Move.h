// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ProjectIncludes.h"
#include "GA_Move.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API UGA_Move : public UGameplayAbility
{
	GENERATED_BODY()
	
private:
	FVector inputDirectionValue;

protected:
	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

public:
	void SetInputDirectionValue(const FVector& inValue) {
		inputDirectionValue = inValue;
	}
};
