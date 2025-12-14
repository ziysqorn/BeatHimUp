// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameState.h"
#include "../GameMode/MainGameMode/MainGameMode.h"

void AMainGameState::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority()) {
		if (AMainGameMode* MainGM = GetWorld()->GetAuthGameMode<AMainGameMode>()) {
			MainGM->OnMatchEndDel.AddUObject(this, &AMainGameState::SetMatchStatus);
		}
	}
}

void AMainGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMainGameState, BossRef);
	DOREPLIFETIME(AMainGameState, MatchStatus);
}

void AMainGameState::OnRep_BossRef(AActor* OldBossRef)
{
}

void AMainGameState::OnRep_MatchStatus(EMatchStatus OldMatchStatus)
{
	if (OnRepMatchStatusDel.IsBound()) {
		OnRepMatchStatusDel.Broadcast(MatchStatus);
	}
}

void AMainGameState::SetMatchStatus(EMatchStatus inMatchStatus)
{
	MatchStatus = inMatchStatus;
}

void AMainGameState::OnBossKilled(AActor* DeadActor, AController* inInstigator, AActor* Causer)
{
	if (HasAuthority()) {
		if (AMainGameMode* MainGM = GetWorld()->GetAuthGameMode<AMainGameMode>()) {
			MainGM->OnBossKilled();
		}
	}
	if (OnBossKilledDel.IsBound()) {
		OnBossKilledDel.Broadcast(DeadActor, inInstigator, Causer);
	}
}

void AMainGameState::OnPlayerKilled(AActor* DeadActor, AController* inInstigator, AActor* Causer)
{
	if (HasAuthority()) {
		if (AMainGameMode* MainGM = GetWorld()->GetAuthGameMode<AMainGameMode>()) {
			MainGM->OnPlayerKilled();
		}
	}
	if (OnPlayerKilledDel.IsBound()) {
		OnPlayerKilledDel.Broadcast(DeadActor, inInstigator, Causer);
	}
}
