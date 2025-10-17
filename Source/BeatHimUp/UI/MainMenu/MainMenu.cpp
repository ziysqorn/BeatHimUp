// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "../../Subsystems/UIManager/UIManagerSubsystem.h"
#include "../../Controller/MainMenuController/MainMenuController.h"
#include "../../PlayerState/MainPlayerState.h"
#include "../../LevelScriptActors/MainMenu/LevelScriptActor_MainMenu.h"

void UMainMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (IsValid(Btn_HideFriendList)) Btn_HideFriendList->OnClicked.AddDynamic(this, &UMainMenu::ToggleFriendlistVisible);
	if (IsValid(Btn_ShowFriendList)) Btn_ShowFriendList->OnClicked.AddDynamic(this, &UMainMenu::ToggleFriendlistVisible);
	if (IsValid(Btn_ToLobby)) Btn_ToLobby->OnClicked.AddDynamic(this, &UMainMenu::ToggleLobbyAndHome);
	if (IsValid(Btn_Home)) Btn_Home->OnClicked.AddDynamic(this, &UMainMenu::ToggleLobbyAndHome);
	if (IsValid(Btn_Logout)) Btn_Logout->OnClicked.AddDynamic(this, &UMainMenu::DisplayLogoutAlert);
}

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	if (UUIManagerSubsystem* UIManager = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>()) {
		UIManager->AddWidget(this);
	}

	if (ALevelScriptActor_MainMenu* MainMenuLevelScript = Cast<ALevelScriptActor_MainMenu>(GetWorld()->GetLevelScriptActor())) {
		MainMenuLevelScript->ResetPlayerPreviewerList();
	}
	InitMainMenu();
}

void UMainMenu::NativeDestruct()
{
	Super::NativeDestruct();

	if (UUIManagerSubsystem* UIManager = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>()) {
		UIManager->PopLastWidget();
	}
}

void UMainMenu::ToggleFriendlistVisible()
{
	if (IsValid(WSwitcher_FriendList)) {
		int currIdx = WSwitcher_FriendList->GetActiveWidgetIndex();
		WSwitcher_FriendList->SetActiveWidgetIndex(FMath::Abs(currIdx - 1));
	}
}

void UMainMenu::ToggleLobbyAndHome()
{
	if (IsValid(WSwitcher_MainScreen)) {
		int currIdx = WSwitcher_MainScreen->GetActiveWidgetIndex();
		WSwitcher_MainScreen->SetActiveWidgetIndex(FMath::Abs(currIdx - 1));
		if (AMainMenuController* PlayerController = this->GetOwningPlayer<AMainMenuController>()) {
			switch (WSwitcher_MainScreen->GetActiveWidgetIndex()) {
			case 1: {
				FInputModeGameAndUI InputMode;
				InputMode.SetHideCursorDuringCapture(false);
				InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
				PlayerController->SetInputMode(InputMode);
				break;
			}
			default: {
				PlayerController->SetInputMode(FInputModeUIOnly());
				break;
			}
			}
		}
	}
}

void UMainMenu::SetCustomInputMode()
{
	if (AMainMenuController* PlayerController = this->GetOwningPlayer<AMainMenuController>()) {
		FInputModeGameAndUI InputMode;
		InputMode.SetHideCursorDuringCapture(false);
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PlayerController->SetInputMode(InputMode);
	}
}

void UMainMenu::ConfirmLogout()
{
	if (AMainMenuController* MainMenuController = this->GetOwningPlayer<AMainMenuController>()) {
		MainMenuController->Client_DisplayLoginScreen();
		if (AMainPlayerState* MainPlayerState = MainMenuController->GetPlayerState<AMainPlayerState>()) {
			MainPlayerState->SetUsername(NAME_None);
		}
	}
}

void UMainMenu::InitMainMenu()
{
	if (AMainMenuController* PlayerController = this->GetOwningPlayer<AMainMenuController>()) {
		PlayerController->SetInputMode(FInputModeUIOnly());
		if (IsValid(WSwitcher_MainScreen)) {
			WSwitcher_MainScreen->SetActiveWidgetIndex(0);
		}
		if (IsValid(WSwitcher_FriendList)) {
			WSwitcher_FriendList->SetActiveWidgetIndex(0);
		}
	}
}

void UMainMenu::DisplayLogoutAlert_Implementation()
{
	if (ScreenAlertSubclass) {
		if (UOnScreenAlert* ScreenAlert = CreateWidget<UOnScreenAlert>(this->GetOwningPlayer(), ScreenAlertSubclass)) {
			if (APlayerController* PlayerController = this->GetOwningPlayer()) PlayerController->SetInputMode(FInputModeUIOnly());
			ScreenAlert->SetOwningPlayer(this->GetOwningPlayer());
			ScreenAlert->SetMessage(FText::FromString("Do you sure you want to logout ?"));
			FOnButtonClickedEvent cancelClicked;
			FOnButtonClickedEvent confirmClicked;
			confirmClicked.AddDynamic(this, &UMainMenu::ConfirmLogout);
			confirmClicked.AddDynamic(ScreenAlert, &UOnScreenAlert::RemoveFromParent);
			cancelClicked.AddDynamic(ScreenAlert, &UOnScreenAlert::RemoveFromParent);
			cancelClicked.AddDynamic(this, &UMainMenu::SetCustomInputMode);
			ScreenAlert->BindConfirmBtn(confirmClicked);
			ScreenAlert->BindCancelBtn(cancelClicked);
			ScreenAlert->AddToViewport(1);
		}
	}
}
