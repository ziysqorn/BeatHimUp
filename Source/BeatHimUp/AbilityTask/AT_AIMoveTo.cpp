// Fill out your copyright notice in the Description page of Project Settings.


#include "AT_AIMoveTo.h"
#include "../Character/BaseCharacter/AICharacter.h"

UAT_AIMoveTo::UAT_AIMoveTo()
{

}

UAT_AIMoveTo* UAT_AIMoveTo::CreateAIMoveTask(UGameplayAbility* Ability, FName InstanceName, AActor* Goal, FVector TargetLocation, float inAcceptanceRadius,
	bool inStopOnOverlap,
	bool inUsePathFinding,
	bool inCanStrafe,
	bool inAllowPartialPath,
	TSubclassOf<UNavigationQueryFilter> inFilter
)
{
	UAT_AIMoveTo* AIMoveTask = NewAbilityTask<UAT_AIMoveTo>(Ability, InstanceName);
	if (AIMoveTask) {
		AIMoveTask->GoalActor = Goal;
		AIMoveTask->TargetLocation = TargetLocation;
		AIMoveTask->AcceptanceRadius = inAcceptanceRadius;
		AIMoveTask->bStopOnOverlap = inStopOnOverlap;
		AIMoveTask->bUsePathFinding = inUsePathFinding;
		AIMoveTask->bCanStrafe = inCanStrafe;
		AIMoveTask->bAllowPartialPath = inAllowPartialPath;
		AIMoveTask->NavigationFilter = inFilter;
	}
	return AIMoveTask;
}

void UAT_AIMoveTo::Activate()
{
	Super::Activate();
	AAIController* AIController = nullptr;
	if (AAICharacter* AICharacter = Cast<AAICharacter>(this->GetOwnerActor())) {
		AIController = AICharacter->GetController<AAIController>();
	}
	if (!IsValid(AIController)) {
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			MoveResult.ExecuteIfBound(EPathFollowingResult::Type::Aborted);
		}
		EndTask();
		return;
	}
	FAIMoveRequest AIMoveReq;
	if (IsValid(GoalActor)) {
		AIMoveReq.SetGoalActor(GoalActor);
	}
	else {
		AIMoveReq.SetGoalLocation(TargetLocation);
	}
	AIMoveReq.SetAcceptanceRadius(AcceptanceRadius);
	AIMoveReq.SetNavigationFilter(NavigationFilter);
	AIMoveReq.SetCanStrafe(bCanStrafe);
	AIMoveReq.SetUsePathfinding(bUsePathFinding);
	AIMoveReq.SetAllowPartialPath(bAllowPartialPath);
	AIMoveReq.SetReachTestIncludesAgentRadius(bStopOnOverlap);

	UPathFollowingComponent* PathFollowingComponent = AIController->GetPathFollowingComponent();
	if (!IsValid(PathFollowingComponent)) {
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			MoveResult.ExecuteIfBound(EPathFollowingResult::Type::Aborted);
		}
		EndTask();
		return;
	}
	MoveFinishedDelHandle = PathFollowingComponent->OnRequestFinished.AddUObject(this, &UAT_AIMoveTo::MoveComplete);
	FPathFollowingRequestResult Result = AIController->MoveTo(AIMoveReq);
	if (Result.Code == EPathFollowingRequestResult::Failed) {
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			MoveResult.ExecuteIfBound(EPathFollowingResult::Type::Aborted);
		}
		EndTask();
		return;
	}
}

void UAT_AIMoveTo::OnDestroy(bool bInOwnerFinished)
{
	if (AAIController* AIController = Cast<AAIController>(GetOwnerActor()))
	{
		if (UPathFollowingComponent* PathComp = AIController->GetPathFollowingComponent())
		{
			PathComp->OnRequestFinished.Remove(MoveFinishedDelHandle);

			if (bInOwnerFinished)
			{
				AIController->StopMovement();
			}
		}
	}
	Super::OnDestroy(bInOwnerFinished);
}

void UAT_AIMoveTo::MoveComplete(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (AAIController* AIController = Cast<AAIController>(GetOwnerActor()))
	{
		if (UPathFollowingComponent* PathComp = AIController->GetPathFollowingComponent())
		{
			PathComp->OnRequestFinished.Remove(MoveFinishedDelHandle);
		}
	}
	if (ShouldBroadcastAbilityTaskDelegates()) {
		MoveResult.ExecuteIfBound(Result.Code);
	}
}
