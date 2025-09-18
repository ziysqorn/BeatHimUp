// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamState.h"

ATeamState::ATeamState()
{
	SetReplicates(true);
}

void ATeamState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
