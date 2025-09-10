// Fill out your copyright notice in the Description page of Project Settings.

#include "GA_Move.h"

void UGA_Move::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (ActorInfo && ActorInfo->OwnerActor.IsValid()) {
		if (ACharacter* character = Cast<ACharacter>(ActorInfo->OwnerActor)) {
			const FRotator Rotation = character->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			const FVector DirectionX = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			const FVector DirectionY = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

			character->AddMovementInput(DirectionX, inputDirectionValue.X);
			character->AddMovementInput(DirectionY, inputDirectionValue.Y);

			EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		}
	}
}
