// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "../../Subsystems/UIManager/UIManagerSubsystem.h"
#include "../../Controller/MainMenuController/MainMenuController.h"
#include "../../PlayerState/MainPlayerState.h"
#include "../../LevelScriptActors/MainMenu/LevelScriptActor_MainMenu.h"
#include "../../ServiceController/UserAccountController/UserAccountController.h"
#include "../../CustomGameInstance/MyGameInstance.h"
#include "../../Subsystems/ServiceControllerSubsystem/ServiceControllerSubsystem.h"

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

	if (UServiceControllerSubsystem* ServiceController = GetGameInstance()->GetSubsystem<UServiceControllerSubsystem>()) {
		if (UMyGameInstance* MyGameInstance = GetGameInstance<UMyGameInstance>()){
			ServiceController->WSMessageRecieveDel.AddUObject(this, &UMainMenu::FriendlistMessageRecvCallback);
			GetWorld()->GetTimerManager().SetTimer(GetFriendlistTimerHandle, FTimerDelegate::CreateUObject(ServiceController->FriendlistController,
				&UFriendlistController::SendFriendlistMessage, Cast<UObject>(this), FName("FriendlistMessageRecvCallback"), MyGameInstance->PlayerInfo.Username), 5.0f, true);
		}
	}

	InitMainMenu();
}

void UMainMenu::NativeDestruct()
{
	Super::NativeDestruct();

	if (GetWorld()->GetTimerManager().IsTimerActive(GetFriendlistTimerHandle)) {
		GetWorld()->GetTimerManager().ClearTimer(GetFriendlistTimerHandle);
	}

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

void UMainMenu::DisplayOnlyCloseAlert_Implementation()
{
	if (UOnlyCloseAlert* OnlyCloseAlert = CreateWidget<UOnlyCloseAlert>(this->GetOwningPlayer(), OnlyCloseAlertSubclass)) {
		OnlyCloseAlert->SetOwningPlayer(this->GetOwningPlayer());
		OnlyCloseAlert->SetMessage(FText::FromString("Unable to start this process yet !"));
		OnlyCloseAlert->AddToViewport(2);
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
		UGameplayStatics::OpenLevel(this, FName("Level_Login"));
	}

}

void UMainMenu::FriendlistMessageRecvCallback_Implementation(const FString& Message)
{
	if (UMyGameInstance* MyGameInstance = GetGameInstance<UMyGameInstance>()) {
		TArray<TSharedPtr<FJsonValue>> jsonObjArr;
		TSharedRef<TJsonReader<>> reader = TJsonReaderFactory<>::Create(Message);
		if (FJsonSerializer::Deserialize(reader, jsonObjArr)) {
			for (const TSharedPtr<FJsonValue>& Value : jsonObjArr) {
				TSharedPtr<FJsonObject> friendJsonObj = Value->AsObject();
				if (friendJsonObj.IsValid()) {
					FPlayerInfo newPlayerInfo;
					newPlayerInfo.Username = FName(friendJsonObj->GetStringField(TEXT("username")));
					newPlayerInfo.isOnline = friendJsonObj->GetBoolField(TEXT("status"));
					MyGameInstance->Friendlist.Add(newPlayerInfo);
				}
			}
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
	if (UMyGameInstance* MyGameInstance = this->GetGameInstance<UMyGameInstance>()) {
		SetUsernameText(FText::FromName(MyGameInstance->PlayerInfo.Username));
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
