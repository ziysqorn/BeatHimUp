// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindRandomPlayer.h"
#include "../Interface/CanBeAggressive.h"

UBTTask_FindRandomPlayer::UBTTask_FindRandomPlayer()
{
    NodeName = TEXT("Find Random Player");
}

EBTNodeResult::Type UBTTask_FindRandomPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    if (AAIController* OwnerAIController = OwnerComp.GetAIOwner()) {
        for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator) {
            if (APlayerController* PlayerController = Iterator->Get()) {
                if (IAbilitySystemInterface* ASI = PlayerController->GetPawn<IAbilitySystemInterface>()) {
                    if (UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent()) {
                        if (!ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Dead")))) {
                            if (ICanBeAggressive* CanBeAggressive = Cast<ICanBeAggressive>(OwnerAIController->GetPawn())) {
                                if (!CanBeAggressive->GetHasBecomeAggressive()) {
                                    CanBeAggressive->SetHasBecomeAggressive(true);
                                }
                            }
                            if (UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent()) {
                                BBComp->SetValueAsObject(FName("Target"), PlayerController->GetPawn());
                            }
                            return EBTNodeResult::Succeeded;
                        }
                    }
                }
            }
        }
    }
    return EBTNodeResult::Failed;
}
