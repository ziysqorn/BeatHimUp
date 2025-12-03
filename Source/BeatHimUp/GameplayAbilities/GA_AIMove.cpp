// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_AIMove.h"
#include "../Character/BaseCharacter/AICharacter.h"

void UGA_AIMove::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (ActorInfo) {
		CurrentSpecHandle = Handle;
		CurrentActorInfo = ActorInfo;
		CurrentActivationInfo = ActivationInfo;
		if (IsValid(GoalActor)) {
			if (UAT_AIMoveTo* AIMoveToTask = UAT_AIMoveTo::CreateAIMoveTask(this, FName("AIMoveToTask"), GoalActor, GoalActor->GetActorLocation(), AcceptanceRadius, bStopOnOverlap, bUsePathFinding, bCanStrafe, bAllowPartialPath, NavigationFilter))
			{
				AIMoveToTask->MoveResult.BindUObject(this, &UGA_AIMove::MoveFinished);
				AIMoveToTask->ReadyForActivation();
			}
		}
		else {
			EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		}
	}
}

void UGA_AIMove::SetupMoveProperties(float inAcceptanceRadius, bool inStopOnOverlap, bool inUsePathFinding, bool inCanStrafe, bool inAllowPartialPath, TSubclassOf<UNavigationQueryFilter> inFilter)
{
	this->AcceptanceRadius = inAcceptanceRadius;
	this->bStopOnOverlap = inStopOnOverlap;
	this->bUsePathFinding = inUsePathFinding;
	this->bCanStrafe = inCanStrafe;
	this->bAllowPartialPath = inAllowPartialPath;
	this->NavigationFilter = inFilter;
}

void UGA_AIMove::MoveFinished(EPathFollowingResult::Type Result)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
