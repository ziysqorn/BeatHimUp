// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginPlayerController.h"

void ALoginPlayerController::BeginPlay()
{
	Super::BeginPlay();
	Client_DisplayLoginScreen();
}

void ALoginPlayerController::Client_DisplayLoginScreen_Implementation()
{
	if (!IsValid(LoginScreen) && LoginScreenSubclass) LoginScreen = CreateWidget<ULoginScreen>(this, LoginScreenSubclass);
	if (IsValid(LoginScreen)) {
		LoginScreen->SetOwningPlayer(this);
		this->SetShowMouseCursor(true);
		LoginScreen->AddToViewport(0);
	}
}