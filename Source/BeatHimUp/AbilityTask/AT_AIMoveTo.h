// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ProjectIncludes.h"
#include "Navigation/PathFollowingComponent.h"
#include "AT_AIMoveTo.generated.h"

/**
 * 
 */
DECLARE_DELEGATE_OneParam(FMoveResultDel, EPathFollowingResult::Type);

UCLASS()
class BEATHIMUP_API UAT_AIMoveTo : public UAbilityTask
{
	GENERATED_BODY()

public:
	UAT_AIMoveTo();

	FMoveResultDel MoveResult;


	static UAT_AIMoveTo* CreateAIMoveTask(UGameplayAbility* Ability, FName InstanceName, AActor* Goal, FVector TargetLocation, float inAcceptanceRadius,
		bool inStopOnOverlap,
		bool inUsePathFinding,
		bool inCanStrafe,
		bool inAllowPartialPath,
		TSubclassOf<UNavigationQueryFilter> inFilter
	);

protected:
	UPROPERTY()
	TObjectPtr<AActor> GoalActor = nullptr;
	FVector TargetLocation;
	float AcceptanceRadius = -0.1f;
	bool bStopOnOverlap = true;
	bool bUsePathFinding = true;
	bool bCanStrafe = false;
	bool bAllowPartialPath = true;
	TSubclassOf<UNavigationQueryFilter> NavigationFilter;

	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;

	void MoveComplete(FAIRequestID RequestID, const FPathFollowingResult& Result);

	FDelegateHandle MoveFinishedDelHandle;
};
