// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuController.h"

void AMainMenuController::BeginPlay()
{
	Super::BeginPlay();

	SetupLoginScreen();
	SetupMainMenu();
}

void AMainMenuController::SetupMainMenu_Implementation()
{
	if (!IsValid(MainMenu)) MainMenu = CreateWidget<UMainMenu>(this, MainMenuSubclass);
}

void AMainMenuController::SetupLoginScreen_Implementation()
{
	if (!IsValid(LoginScreen)) LoginScreen = CreateWidget<ULoginScreen>(this, LoginScreenSubclass);
	if (IsValid(LoginScreen)) {
		LoginScreen->SetOwningPlayer(this);
		LoginScreen->AddToViewport(0);
		this->SetShowMouseCursor(true);
	}
}
