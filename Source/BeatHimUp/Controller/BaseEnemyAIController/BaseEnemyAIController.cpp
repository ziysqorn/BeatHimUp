// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemyAIController.h"
#include "../../Character/BaseCharacter/AICharacter.h"
#include "../../Character/MainCharacter/MainCharacter.h"

ABaseEnemyAIController::ABaseEnemyAIController()
{
	SetupPerceptionComponent();
}

void ABaseEnemyAIController::OnPossess(APawn* inPawn)
{
	Super::OnPossess(inPawn);

	if (AAICharacter* AICharacter = Cast<AAICharacter>(inPawn)) {
		if (UBehaviorTree* Tree = AICharacter->GetBehaviorTree()) {
			UBlackboardComponent* temp = nullptr;
			UseBlackboard(Tree->GetBlackboardAsset(), temp);
			Blackboard = temp;
			RunBehaviorTree(Tree);
		}
	}
}

void ABaseEnemyAIController::SetupPerceptionComponent()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightSenseConfig"));
	if (IsValid(SightConfig)) {
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent")));
		SightConfig->SightRadius = 700.f;
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 25.f;
		SightConfig->PeripheralVisionAngleDegrees = 90.f;
		SightConfig->SetMaxAge(5.f);
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 525.f;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		GetPerceptionComponent()->SetDominantSense(SightConfig->GetSenseImplementation());
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ABaseEnemyAIController::OnTargetDetected);

		GetPerceptionComponent()->ConfigureSense(*SightConfig);
	}
}

void ABaseEnemyAIController::OnTargetDetected(AActor* actor, FAIStimulus Stimulus)
{
	if (AMainCharacter* MainCharacter = Cast<AMainCharacter>(actor)) {
		if (GetBlackboardComponent()) {
			if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(MainCharacter)) {
				if (UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent()) {
					if (!ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Dead")))) {
						if (AActor* Target = Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FName("Target")))) {
							if (this->GetPawn()) {
								FVector VectorToDetectedActor = MainCharacter->GetActorLocation() - this->GetPawn()->GetActorLocation();
								FVector VectortoCurrentTarget = Target->GetActorLocation() - this->GetPawn()->GetActorLocation();
								float DistanceToDetectedActor = VectorToDetectedActor.Length();
								float DistanceToCurrentTarget = VectortoCurrentTarget.Length();
								/*GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Format(TEXT("To detected actor: {0}"), { DistanceToDetectedActor }));
								GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Format(TEXT("To current target: {0}"), { DistanceToCurrentTarget }));*/
								if (DistanceToDetectedActor < DistanceToCurrentTarget) {
									if (IHaveSpecialDeath* TargetHaveSpecialDeath = Cast<IHaveSpecialDeath>(Target)) {
										TargetHaveSpecialDeath->OnDeath().RemoveAll(this);
									}
									if (IHaveSpecialDeath* HaveSpecialDeath = Cast<IHaveSpecialDeath>(MainCharacter)) {
										HaveSpecialDeath->OnDeath().AddUObject(this, &ABaseEnemyAIController::OnTargetDeath);
										GetBlackboardComponent()->SetValueAsObject(FName("Target"), MainCharacter);
									}
								}
							}
						}
						else {
							if (IHaveSpecialDeath* HaveSpecialDeath = Cast<IHaveSpecialDeath>(MainCharacter)) {
								HaveSpecialDeath->OnDeath().AddUObject(this, &ABaseEnemyAIController::OnTargetDeath);
								GetBlackboardComponent()->SetValueAsObject(FName("Target"), MainCharacter);
							}
						}
					}
				}
			}
		}
	}
}

void ABaseEnemyAIController::OnTargetDeath(AActor* DeadTarget)
{
	if (GetBlackboardComponent()) {
		if (AActor* Target = Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FName("Target")))) 
		{
			if(Target == DeadTarget)
				GetBlackboardComponent()->SetValueAsObject(FName("Target"), nullptr);
		}
	}
}
