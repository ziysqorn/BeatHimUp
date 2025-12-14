// Fill out your copyright notice in the Description page of Project Settings.


#include "MainController.h"
#include "../../CustomGameState/MainGameState.h"
#include "../../GameMode/MainGameMode/MainGameMode.h"

AMainController::AMainController()
{
	PlayerHUDComp = CreateDefaultSubobject<UPlayerHUDComponent>(FName("PlayerHUDComp"));
}

void AMainController::BeginPlay()
{
	Super::BeginPlay();
}

void AMainController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}

void AMainController::AcknowledgePossession(APawn* aPawn)
{
	Super::AcknowledgePossession(aPawn);

	if (IsValid(PlayerHUDComp)) {
		PlayerHUDComp->Client_AddHUD();
		if (AMainGameState* MainGS = GetWorld()->GetGameState<AMainGameState>()) {
			MainGS->OnRepMatchStatusDel.AddUObject(PlayerHUDComp, &UPlayerHUDComponent::DisplayMatchStatusMessage);
		}
	}
}

void AMainController::SpectatePlayer()
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator) {
		if (Iterator->IsValid()) {
			if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(Iterator->Get()->GetPawn())) {
				if (UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent()) {
					if (!ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Dead")))) {
						CurSpectatedPlayerIdx = Iterator.GetIndex();
						CurrentSpectatedPlayer = Iterator->Get(false);
						SetViewTargetWithBlend(Iterator->Get());
						return;
					}
				}
			}
		}
	}
}

void AMainController::Server_RequestEndGame_Implementation(EMatchStatus inMatchStatus)
{
	if (AMainGameMode* MainGM = GetWorld()->GetAuthGameMode<AMainGameMode>()) {
		MainGM->EndMatch(inMatchStatus);
	}
}

void AMainController::Server_SpectateNextPlayer_Implementation()
{
	if (CurSpectatedPlayerIdx != -1) {
		int NumPlayers = GetWorld()->GetNumPlayerControllers();
		int curIdx = CurSpectatedPlayerIdx + 1;
		while (true) {
			if (curIdx == NumPlayers) {
				curIdx = 0;
			}
			if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), curIdx)) {
				if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(PC->GetPawn())) {
					if (UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent()) {
						if (!ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Dead")))) {
							CurSpectatedPlayerIdx = curIdx;
							CurrentSpectatedPlayer = PC;
							SetViewTargetWithBlend(PC);
							return;
						}
					}
				}
			}
			++curIdx;
		}
	}
}
