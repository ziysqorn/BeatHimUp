// Fill out your copyright notice in the Description page of Project Settings.


#include "MainController.h"
#include "../../CustomGameState/MainGameState.h"

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
	}
}
