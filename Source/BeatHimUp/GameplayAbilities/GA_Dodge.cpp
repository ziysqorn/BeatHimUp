// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Dodge.h"

void UGA_Dodge::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (ActorInfo && IsValid(DodgeMontage)) {
		CurrentSpecHandle = Handle;
		CurrentActorInfo = ActorInfo;
		CurrentActivationInfo = ActivationInfo;
		Client_RequestRotateOwner();
		if (UAbilityTask_PlayMontageAndWait* PlayDodgeMontageAndWait = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("PlayDodgeMontageAndWait"), DodgeMontage)) {
			PlayDodgeMontageAndWait->OnCompleted.AddDynamic(this, &UGA_Dodge::DodgeEnd);
			PlayDodgeMontageAndWait->ReadyForActivation();
			//CommitAbility(Handle, ActorInfo, ActivationInfo);
		}
	}
}

void UGA_Dodge::DodgeEnd()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_Dodge::Server_RotateOwnerByMovementInput_Implementation(FVector LastInputMovementVector)
{
	if (CurrentActorInfo) {
		if (ACharacter* character = Cast<ACharacter>(CurrentActorInfo->OwnerActor)) {
			character->SetActorRotation(LastInputMovementVector.Rotation());
		}
	}
}

void UGA_Dodge::Client_RequestRotateOwner_Implementation()
{
	if (CurrentActorInfo) {
		if (ACharacter* character = Cast<ACharacter>(CurrentActorInfo->OwnerActor)) {
			Server_RotateOwnerByMovementInput(character->GetLastMovementInputVector());
		}
	}
}

bool UGA_Dodge::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	return CheckCost(Handle, ActorInfo);
}
