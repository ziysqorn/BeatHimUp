// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ProjectIncludes.h"
#include "BTTask_ActivateAttack.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API UBTTask_ActivateAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_ActivateAttack();

protected:
	UPROPERTY(EditAnywhere, Category = "AttackAbilitySubclass")
	TSubclassOf<UGameplayAbility> AttackAbilitySubclass;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UFUNCTION()
	void SetTaskResultSucceeded(UGameplayAbility* Ability);

private:
	UPROPERTY()
	UBehaviorTreeComponent* CachedOwnerComp = nullptr;
};
