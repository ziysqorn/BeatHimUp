// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ProjectIncludes.h"
#include "BTTask_AIMove.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API UBTTask_AIMove : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_AIMove();

protected:
	UPROPERTY(EditAnywhere, Category = "MoveOptions")
	float AcceptanceRadius = -0.1f;

	UPROPERTY(EditAnywhere, Category = "MoveOptions")
	bool bStopOnOverlap = true;

	UPROPERTY(EditAnywhere, Category = "MoveOptions")
	bool bUsePathFinding = true;

	UPROPERTY(EditAnywhere, Category = "MoveOptions")
	bool bCanStrafe = false;

	UPROPERTY(EditAnywhere, Category = "MoveOptions")
	bool bAllowPartialPath = true;

	UPROPERTY(EditAnywhere, Category = "MoveOptions")
	TSubclassOf<UNavigationQueryFilter> NavigationFilter;

	UPROPERTY(EditAnywhere, Category = "AbilitySubclass")
	TSubclassOf<UGameplayAbility> AbilitySubclass;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UFUNCTION()
	void SetTaskResultSucceeded(UGameplayAbility* Ability);

private:
	UPROPERTY()
	UBehaviorTreeComponent* CachedOwnerComp = nullptr;
};
