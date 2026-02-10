// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "../../Subsystems/UIManager/UIManagerSubsystem.h"
#include "../../Controller/MainMenuController/MainMenuController.h"
#include "../../CustomGameInstance/MyGameInstance.h"
#include "../../Subsystems/ServiceControllerSubsystem/ServiceControllerSubsystem.h"

void UMainMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (IsValid(Btn_ToLobby)) Btn_ToLobby->OnClicked.AddDynamic(this, &UMainMenu::ToggleLobbyAndHome);
	if (IsValid(Btn_Home)) Btn_Home->OnClicked.AddDynamic(this, &UMainMenu::ToggleLobbyAndHome);
	if (IsValid(Btn_Play)) Btn_Play->OnClicked.AddDynamic(this, &UMainMenu::StartGame);
	if (UMyGameInstance* MyGameInstance = GetGameInstance<UMyGameInstance>()) {
		MyGameInstance->OnLobbyUpdateDel.AddUObject(this, &UMainMenu::UpdateMenuAccordingToLobbyUpdate);
	}
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

void UMainMenu::DisplayOnlyCloseAlert_Implementation(const FString& message)
{
	if (IsValid(DA_UI)) {
		if (UOnlyCloseAlert* OnlyCloseAlert = CreateWidget<UOnlyCloseAlert>(this->GetOwningPlayer(), *DA_UI->UISubclassMap.Find(FName("OnlyCloseAlert")))) {
			OnlyCloseAlert->SetOwningPlayer(this->GetOwningPlayer());
			OnlyCloseAlert->SetMessage(FText::FromString(message));
			OnlyCloseAlert->AddToViewport(2);
		}
	}
}

void UMainMenu::InitMainMenu()
{
	if (IsValid(WSwitcher_MainScreen)) {
		WSwitcher_MainScreen->SetActiveWidgetIndex(0);
	}
	if (IsValid(NamedSlot_SideInfo)) {
		if (IsValid(SideInfoSubclass)) {
			if (UUserWidget* SideInfo = CreateWidget<UUserWidget>(this->GetOwningPlayer(), SideInfoSubclass)) {
				NamedSlot_SideInfo->AddChild(SideInfo);
			}
		}
	}
	if (UMyGameInstance* MyGameInstance = GetGameInstance<UMyGameInstance>()) {
		UpdateMenuAccordingToLobbyUpdate(MyGameInstance->GetLobbyInfo());
	}
}

FReply UMainMenu::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (UUIManagerSubsystem* UIManager = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>()) {
		UIManager->HideCtxMenu();
	}
	return FReply::Unhandled();
}

void UMainMenu::UpdateMenuAccordingToLobbyUpdate(const FLobbyInfo& LobbyInfo)
{
	if (UMyGameInstance* MyGameInstance = GetGameInstance<UMyGameInstance>()) {
		if (MyGameInstance->GetPlayerInfo().Username.IsEqual(LobbyInfo.Leader_Username)) {
			if (IsValid(Btn_Play)) {
				Btn_Play->SetIsEnabled(true);
			}
		}
		else {
			if (IsValid(Btn_Play)) {
				Btn_Play->SetIsEnabled(false);
			}
		}
	}
}

void UMainMenu::StartGame()
{
	if (AMainMenuController* MainMenuController = this->GetOwningPlayer<AMainMenuController>()) {
		MainMenuController->StartGame();
	}
}
