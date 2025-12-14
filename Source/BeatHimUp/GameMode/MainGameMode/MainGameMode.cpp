// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameMode.h"

void AMainGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AMainGameMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AMainGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void AMainGameMode::OnBossKilled()
{
	StartEndMatch(EMatchStatus::MATCH_WON);
}

void AMainGameMode::OnPlayerKilled()
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator) {
		if (Iterator->IsValid()) {
			if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(Iterator->Get()->GetPawn())) {
				if (UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent()) {
					if (!ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Dead")))) {
						return;
					}
				}
			}
		}
	}
	StartEndMatch(EMatchStatus::MATCH_LOST);
}

void AMainGameMode::EndMatch(EMatchStatus inMatchStatus)
{
	switch (inMatchStatus) {
	case EMatchStatus::MATCH_WON:
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("CONGRATULATIONS ! YOU WON !!!!!!!!!"));
		break;
	case EMatchStatus::MATCH_LOST:
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("SORRY ! YOU LOSE......."));
		break;
	}
}

void AMainGameMode::StartEndMatch(EMatchStatus inMatchStatus)
{
	if (OnMatchEndDel.IsBound()) {
		OnMatchEndDel.Broadcast(inMatchStatus);
	}
}
