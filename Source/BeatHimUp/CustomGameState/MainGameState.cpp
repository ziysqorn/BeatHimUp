// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameState.h"

void AMainGameState::BeginPlay()
{
	Super::BeginPlay();
}

void AMainGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMainGameState, BossRef);
}

void AMainGameState::OnRep_BossRef(AActor* OldBossRef)
{
}
