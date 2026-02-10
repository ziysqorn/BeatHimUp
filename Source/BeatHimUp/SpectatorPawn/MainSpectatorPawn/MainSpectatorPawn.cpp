// Fill out your copyright notice in the Description page of Project Settings.


#include "MainSpectatorPawn.h"
#include "../../Controller/MainController/MainController.h"

AMainSpectatorPawn::AMainSpectatorPawn()
{
	bAddDefaultMovementBindings = false;
}

void AMainSpectatorPawn::BeginPlay()
{
	Super::BeginPlay();

	SetupMappingContext();

	FVector SpectatorPawnOriginalPos = this->GetActorLocation();
	SpectatorPawnOriginalPos.Z += 100.f;
	SetActorLocation(SpectatorPawnOriginalPos);
}

void AMainSpectatorPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EIComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		if (AMainController* MC = this->GetController<AMainController>()) {
			if (IsValid(IA_NextSpectatedPlayer)) EIComponent->BindAction(IA_NextSpectatedPlayer, ETriggerEvent::Triggered, MC, &AMainController::ServerViewNextPlayer);
			if (IsValid(IA_PauseGame)) EIComponent->BindAction(IA_PauseGame, ETriggerEvent::Triggered, MC, &AMainController::PauseGame);
		}
	}
}

void AMainSpectatorPawn::ViewNextPlayer()
{
	if (APlayerController* PC = this->GetController<APlayerController>()) {
		PC->ServerViewNextPlayer();
	}
}

void AMainSpectatorPawn::SetupMappingContext()
{
	if (APlayerController* PlayerController = this->GetController<APlayerController>()) {
		if (UEnhancedInputLocalPlayerSubsystem* EISubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			EISubsystem->AddMappingContext(MC_SpectatorMode, 0);
		}
	}
}
