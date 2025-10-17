// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "../../Subsystems/UIManager/UIManagerSubsystem.h"
#include "../../Controller/MainMenuController/MainMenuController.h"
#include "../../PlayerState/MainPlayerState.h"

void UMainMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (IsValid(Btn_HideFriendList)) Btn_HideFriendList->OnClicked.AddDynamic(this, &UMainMenu::ToggleFriendlistVisible);
	if (IsValid(Btn_ShowFriendList)) Btn_ShowFriendList->OnClicked.AddDynamic(this, &UMainMenu::ToggleFriendlistVisible);
	if (IsValid(Btn_ToLobby)) Btn_ToLobby->OnClicked.AddDynamic(this, &UMainMenu::ToggleMainScreenAndHome);
	if (IsValid(Btn_Home)) Btn_Home->OnClicked.AddDynamic(this, &UMainMenu::ToggleMainScreenAndHome);
	SetIsFocusable(true);
}

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	if (UUIManagerSubsystem* UIManager = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>()) {
		UIManager->AddWidget(this);
	}
	SetFocus();
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

void UMainMenu::ToggleMainScreenAndHome()
{
	if (IsValid(WSwitcher_MainScreen)) {
		int currIdx = WSwitcher_MainScreen->GetActiveWidgetIndex();
		WSwitcher_MainScreen->SetActiveWidgetIndex(FMath::Abs(currIdx - 1));
	}
}
