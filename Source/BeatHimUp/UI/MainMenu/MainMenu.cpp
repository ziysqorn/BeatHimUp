// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "../../Subsystems/UIManager/UIManagerSubsystem.h"
#include "../../Controller/MainMenuController/MainMenuController.h"
#include "../../ServiceController/UserAccountController/UserAccountController.h"
#include "../../CustomGameInstance/MyGameInstance.h"
#include "../../Subsystems/ServiceControllerSubsystem/ServiceControllerSubsystem.h"
#include "../../UI/FriendTag/FriendTag.h"

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
		if (APlayerController* PlayerController = this->GetOwningPlayer()) {
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

void UMainMenu::DisplayOnlyCloseAlert_Implementation()
{
	if (IsValid(DA_UI)) {
		if (UOnlyCloseAlert* OnlyCloseAlert = CreateWidget<UOnlyCloseAlert>(this->GetOwningPlayer(), *DA_UI->UISubclassMap.Find(FName("OnlyCloseAlert")))) {
			OnlyCloseAlert->SetOwningPlayer(this->GetOwningPlayer());
			OnlyCloseAlert->SetMessage(FText::FromString("Unable to start this process yet !"));
			OnlyCloseAlert->AddToViewport(2);
		}
	}
}

void UMainMenu::SetCustomInputMode()
{
	if (APlayerController* PlayerController = this->GetOwningPlayer<APlayerController>()) {
		FInputModeGameAndUI InputMode;
		InputMode.SetHideCursorDuringCapture(false);
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PlayerController->SetInputMode(InputMode);
	}
}

void UMainMenu::ConfirmLogout()
{
	if (APlayerController* MainMenuController = this->GetOwningPlayer()) {
		UGameplayStatics::OpenLevel(this, FName("Level_Login"));
	}

}

void UMainMenu::InitMainMenu()
{
	if (IsValid(WSwitcher_MainScreen)) {
		WSwitcher_MainScreen->SetActiveWidgetIndex(0);
	}
	if (IsValid(WSwitcher_FriendList)) {
		WSwitcher_FriendList->SetActiveWidgetIndex(0);
	}
}

FReply UMainMenu::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (UUIManagerSubsystem* UIManager = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>()) {
		UIManager->HideCtxMenu();
	}
	return FReply::Unhandled();
}

void UMainMenu::SetupFriendlist(const TArray<FPlayerInfo>& Friendlist)
{
	if (IsValid(ScrollBox_Friendlist) && IsValid(DA_UI)) {
		ScrollBox_Friendlist->ClearChildren();
		for (const FPlayerInfo& Player : Friendlist) {
			if (UFriendTag* FriendTag = CreateWidget<UFriendTag>(this->GetOwningPlayer(), *DA_UI->UISubclassMap.Find(FName("FriendTag")))) {
				FriendTag->SetUsernameText(FText::FromName(Player.Username));
				Player.isOnline ? FriendTag->SetTextOnline() : FriendTag->SetTextOffline();
				ScrollBox_Friendlist->AddChild(FriendTag);
			}
		}
	}
}

void UMainMenu::DisplayLogoutAlert_Implementation()
{
	if (IsValid(DA_UI)) {
		if (UOnScreenAlert* ScreenAlert = CreateWidget<UOnScreenAlert>(this->GetOwningPlayer(), *DA_UI->UISubclassMap.Find(FName("OnScreenAlert")))) {
			if (APlayerController* PlayerController = this->GetOwningPlayer()) 
				PlayerController->SetInputMode(FInputModeUIOnly());
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
