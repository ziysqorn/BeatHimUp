// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerState.h"

AMainPlayerState::AMainPlayerState()
{
	SetReplicates(true);
}

void AMainPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
