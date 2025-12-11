// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindRandomPlayer.h"
#include "../Interface/CanBeAggressive.h"
#include "../CustomGameState/MainGameState.h"

UBTTask_FindRandomPlayer::UBTTask_FindRandomPlayer()
{
    NodeName = TEXT("Find Random Player");
}

EBTNodeResult::Type UBTTask_FindRandomPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    if (AAIController* OwnerAIController = OwnerComp.GetAIOwner()) {
        int PlayerControllersNum = UGameplayStatics::GetNumPlayerControllers(OwnerAIController->GetWorld());
        int RandomPlayerControllerIdx = FMath::RandRange(0, PlayerControllersNum);
        if (APlayerController* ChosenPlayerController = UGameplayStatics::GetPlayerController(OwnerAIController->GetWorld(), RandomPlayerControllerIdx)) {
            if (ICanBeAggressive* CanBeAggressive = Cast<ICanBeAggressive>(OwnerAIController->GetPawn())) {
                if (!CanBeAggressive->GetHasBecomeAggressive()) {
                    if (AMainGameState* MainGS = GetWorld()->GetGameState<AMainGameState>()) {
                        MainGS->SetBossRef(OwnerAIController->GetPawn());
                        CanBeAggressive->SetHasBecomeAggressive(true);
                    }
                }
            }
            if (UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent()) {
                BBComp->SetValueAsObject(FName("Target"), ChosenPlayerController->GetPawn());
            }
            return EBTNodeResult::Succeeded;
        }
    }
    return EBTNodeResult::Failed;
}
