// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "BaseEnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API ABaseEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	ABaseEnemyAIController();

	void OnTargetDeath(AActor* DeadTarget);

protected:
	void OnPossess(APawn* inPawn) override;

private:
	TObjectPtr<UAISenseConfig_Sight> SightConfig = nullptr;

	void SetupPerceptionComponent();

	UFUNCTION()
	void OnTargetDetected(AActor* actor, FAIStimulus Stimulus);
};
