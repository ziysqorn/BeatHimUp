// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ProjectIncludes.h"
#include "../AbilityTask/AT_AIMoveTo.h"
#include "GA_AIMove.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API UGA_AIMove : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TObjectPtr<AActor> GoalActor = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "MoveOptions")
	float AcceptanceRadius = -0.1f;

	UPROPERTY(EditDefaultsOnly, Category = "MoveOptions")
	bool bStopOnOverlap = true;

	UPROPERTY(EditDefaultsOnly, Category = "MoveOptions")
	bool bUsePathFinding = true;

	UPROPERTY(EditDefaultsOnly, Category = "MoveOptions")
	bool bCanStrafe = false;

	UPROPERTY(EditDefaultsOnly, Category = "MoveOptions")
	bool bAllowPartialPath = true;

	UPROPERTY(EditDefaultsOnly, Category = "MoveOptions")
	TSubclassOf<UNavigationQueryFilter> NavigationFilter;

	void MoveFinished(EPathFollowingResult::Type Result);

	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

public:
	void SetupMoveProperties(float inAcceptanceRadius, bool inStopOnOverlap, bool inUsePathFinding, bool inCanStrafe, bool inAllowPartialPath, TSubclassOf<UNavigationQueryFilter> inFilter);

	void SetGoalActor(AActor* Actor) {
		GoalActor = Actor;
	}
};
