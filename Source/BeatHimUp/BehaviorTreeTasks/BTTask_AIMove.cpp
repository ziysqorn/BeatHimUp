// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_AIMove.h"
#include "../Character/BaseCharacter/AICharacter.h"
#include "../GameplayAbilities/GA_AIMove.h"

UBTTask_AIMove::UBTTask_AIMove()
{
	NodeName = TEXT("Activate AI Move Ability");
}

EBTNodeResult::Type UBTTask_AIMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAIController* Controller = OwnerComp.GetAIOwner()) {
		if (AAICharacter* AICharacter = Controller->GetPawn<AAICharacter>()) {
			if (UAbilitySystemComponent* ASC = AICharacter->GetAbilitySystemComponent()) {
				if (AbilitySubclass) {
					if (FGameplayAbilitySpec* GASpec = ASC->FindAbilitySpecFromClass(AbilitySubclass)) {
						if (UGA_AIMove* AIMoveAbility = Cast<UGA_AIMove>(GASpec->GetPrimaryInstance())) {
							if (UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent()) {
								if (AActor* Target = Cast<AActor>(BlackboardComp->GetValueAsObject(BlackboardKey.SelectedKeyName))) {
									CachedOwnerComp = &OwnerComp;
									AIMoveAbility->OnGameplayAbilityEnded.AddUObject(this, &UBTTask_AIMove::SetTaskResultSucceeded);
									AIMoveAbility->SetupMoveProperties(AcceptanceRadius, bStopOnOverlap, bUsePathFinding, bCanStrafe, bAllowPartialPath, NavigationFilter);
									AIMoveAbility->SetGoalActor(Target);
									if (ASC->TryActivateAbilityByClass(AbilitySubclass))
										return EBTNodeResult::InProgress;
								}
							}
						}
					}
				}
			}
		}
	}
	return EBTNodeResult::Failed;
}

void UBTTask_AIMove::SetTaskResultSucceeded(UGameplayAbility* Ability)
{
	FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
}
