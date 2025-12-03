// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ActivateAbility.h"
#include "../Character/BaseCharacter/AICharacter.h"
#include "../GameplayAbilities/GA_MonsterSwiping.h"

UBTTask_ActivateAbility::UBTTask_ActivateAbility()
{
	NodeName = TEXT("Activate Gameplay Ability");
}

EBTNodeResult::Type UBTTask_ActivateAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAIController* Controller = OwnerComp.GetAIOwner()) {
		if (AAICharacter* AICharacter = Controller->GetPawn<AAICharacter>()) {
			if (UAbilitySystemComponent* ASC = AICharacter->GetAbilitySystemComponent()) {
				if (AbilitySubclass) {
					if (ASC->TryActivateAbilityByClass(AbilitySubclass)) {
						if (FGameplayAbilitySpec* GASpec = ASC->FindAbilitySpecFromClass(AbilitySubclass)) {
							if (UGameplayAbility* Ability = Cast<UGameplayAbility>(GASpec->GetPrimaryInstance())) {
								CachedOwnerComp = &OwnerComp;
								Ability->OnGameplayAbilityEnded.AddUObject(this, &UBTTask_ActivateAbility::SetTaskResultSucceeded);
								return EBTNodeResult::InProgress;
							}
						}
					}
				}
			}
		}
	}
	return EBTNodeResult::Failed;
}

void UBTTask_ActivateAbility::SetTaskResultSucceeded(UGameplayAbility* Ability)
{
	FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
}
