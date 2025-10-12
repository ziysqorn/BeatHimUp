// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuController.h"
#include "../../Subsystems/UIManager/UIManagerSubsystem.h"

void AMainMenuController::BeginPlay()
{
	Super::BeginPlay();

	Client_DisplayLoginScreen();
	SetInputMode(FInputModeUIOnly());
}


void AMainMenuController::Client_DisplayMainMenu_Implementation()
{
	if (!IsValid(MainMenu) && MainMenuSubclass) MainMenu = CreateWidget<UMainMenu>(this, MainMenuSubclass);
	if (IsValid(MainMenu)) {
		if (IsValid(LoginScreen)) {
			LoginScreen->RemoveFromParent();
		}
		MainMenu->SetOwningPlayer(this);
		MainMenu->AddToViewport(0);
		this->SetShowMouseCursor(true);
	}
}

void AMainMenuController::Client_DisplayLoginScreen_Implementation()
{
	if (!IsValid(LoginScreen) && LoginScreenSubclass) LoginScreen = CreateWidget<ULoginScreen>(this, LoginScreenSubclass);
	if (IsValid(LoginScreen)) {
		if (IsValid(MainMenu)) {
			MainMenu->RemoveFromParent();
		}
		LoginScreen->SetOwningPlayer(this);
		LoginScreen->AddToViewport(0);
		this->SetShowMouseCursor(true);
	}
}
