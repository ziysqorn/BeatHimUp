// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ActivateAttack.h"
#include "../Character/BaseCharacter/AICharacter.h"
#include "../GameplayAbilities/GA_MonsterSwiping.h"

UBTTask_ActivateAttack::UBTTask_ActivateAttack()
{
	NodeName = TEXT("Activate Attack Ability");
}

EBTNodeResult::Type UBTTask_ActivateAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAIController* Controller = OwnerComp.GetAIOwner()) {
		if (AAICharacter* AICharacter = Controller->GetPawn<AAICharacter>()) {
			if (UAbilitySystemComponent* ASC = AICharacter->GetAbilitySystemComponent()) {
				if (AttackAbilitySubclass) {
					if (ASC->TryActivateAbilityByClass(AttackAbilitySubclass)) {
						if (FGameplayAbilitySpec* GASpec = ASC->FindAbilitySpecFromClass(AttackAbilitySubclass)) {
							if (UGA_MonsterSwiping* MonsterSwiping = Cast<UGA_MonsterSwiping>(GASpec->GetPrimaryInstance())) {
								CachedOwnerComp = &OwnerComp;
								MonsterSwiping->OnGameplayAbilityEnded.AddUObject(this, &UBTTask_ActivateAttack::SetTaskResultSucceeded);
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

void UBTTask_ActivateAttack::SetTaskResultSucceeded(UGameplayAbility* Ability) 
{
	FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
}
