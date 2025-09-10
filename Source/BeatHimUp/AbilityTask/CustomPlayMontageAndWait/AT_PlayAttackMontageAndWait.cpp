// Fill out your copyright notice in the Description page of Project Settings.


#include "AT_PlayAttackMontageAndWait.h"

void UAT_PlayAttackMontageAndWait::Activate()
{
	if (IsValid(Ability)) {
		if (APlayerState* PlayerState = Ability->GetActorInfo().PlayerController->GetPlayerState<APlayerState>()) {
			GEngine->AddOnScreenDebugMessage(0, 5.0f, FColor::Blue, FString::Format(TEXT("The character {0} is attacking"), { PlayerState->GetPlayerName() }));
		}
	}
}
