// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuController.h"
#include "../../Subsystems/UIManager/UIManagerSubsystem.h"

void AMainMenuController::BeginPlay()
{
	Super::BeginPlay();

	SetupLoginScreen();
	SetupMainMenu();
	SetInputMode(FInputModeUIOnly());
}

void AMainMenuController::SetupMainMenu_Implementation()
{
	if (!IsValid(MainMenu) && MainMenuSubclass) MainMenu = CreateWidget<UMainMenu>(this, MainMenuSubclass);
}

void AMainMenuController::SetupLoginScreen_Implementation()
{
	if (!IsValid(LoginScreen) && LoginScreenSubclass) LoginScreen = CreateWidget<ULoginScreen>(this, LoginScreenSubclass);
	if (IsValid(LoginScreen)) {
		LoginScreen->SetOwningPlayer(this);
		LoginScreen->AddToViewport(0);
		this->SetShowMouseCursor(true);
		if (UUIManagerSubsystem* UIManager = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>()) {
			UIManager->AddWidget(LoginScreen);
		}
	}
}
