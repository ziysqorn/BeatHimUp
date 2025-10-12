// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "../../Subsystems/UIManager/UIManagerSubsystem.h"
#include "../../Controller/MainMenuController/MainMenuController.h"
#include "../../PlayerState/MainPlayerState.h"

void UMainMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	if (UUIManagerSubsystem* UIManager = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>()) {
		UIManager->AddWidget(this);
	}

	if (AMainMenuController* MainMenuController = this->GetOwningPlayer<AMainMenuController>()) {
		if (AMainPlayerState* MainPlayerState = MainMenuController->GetPlayerState<AMainPlayerState>()) {
			SetUsernameText(FText::FromName(MainPlayerState->GetUsername()));
		}
	}
}

void UMainMenu::NativeDestruct()
{
	Super::NativeDestruct();

	if (UUIManagerSubsystem* UIManager = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>()) {
		UIManager->PopLastWidget();
	}
}
