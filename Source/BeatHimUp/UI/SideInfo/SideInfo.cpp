// Fill out your copyright notice in the Description page of Project Settings.


#include "SideInfo.h"
#include "../../UI/FriendTag/FriendTag.h"
#include "../../UI/ReceivedFriendRequestPanel/ReceivedFriendRequestPanel.h"
#include "../../UI/LobbyInvitationPanel/LobbyInvitationPanel.h"
#include "../../CustomGameInstance/MyGameInstance.h"
#include "../../Controller/MainMenuController/MainMenuController.h"

void USideInfo::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (IsValid(Btn_HideFriendList)) Btn_HideFriendList->OnClicked.AddDynamic(this, &USideInfo::ToggleFriendlistVisible);
	if (IsValid(Btn_ShowFriendList)) Btn_ShowFriendList->OnClicked.AddDynamic(this, &USideInfo::ToggleFriendlistVisible);
	if (IsValid(Btn_Logout)) Btn_Logout->OnClicked.AddDynamic(this, &USideInfo::DisplayLogoutAlert);
	if (IsValid(Btn_AddFriend)) Btn_AddFriend->OnClicked.AddDynamic(this, &USideInfo::SendFriendRequest);
	if (IsValid(Btn_PreviousRequest)) Btn_PreviousRequest->OnClicked.AddDynamic(this, &USideInfo::PrevRequest);
	if (IsValid(Btn_NextRequest)) Btn_NextRequest->OnClicked.AddDynamic(this, &USideInfo::NextRequest);
}

void USideInfo::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(WSwitcher_FriendList)) {
		WSwitcher_FriendList->SetActiveWidgetIndex(0);
	}
}

void USideInfo::NativeDestruct()
{
	Super::NativeDestruct();
}

void USideInfo::ToggleFriendlistVisible() 
{
	if (IsValid(WSwitcher_FriendList)) {
		int currIdx = WSwitcher_FriendList->GetActiveWidgetIndex();
		WSwitcher_FriendList->SetActiveWidgetIndex(FMath::Abs(currIdx - 1));
	}
}

void USideInfo::ConfirmLogout()
{
	if (APlayerController* MainMenuController = this->GetOwningPlayer()) {
		UGameplayStatics::OpenLevel(this, FName("Level_Login"));
	}
}

void USideInfo::DisplayLogoutAlert_Implementation()
{
	if (IsValid(DA_UI)) {
		if (UOnScreenAlert* ScreenAlert = CreateWidget<UOnScreenAlert>(this->GetOwningPlayer(), *DA_UI->UISubclassMap.Find(FName("OnScreenAlert")))) {
			if (APlayerController* PlayerController = this->GetOwningPlayer())
				PlayerController->SetInputMode(FInputModeUIOnly());
			ScreenAlert->SetOwningPlayer(this->GetOwningPlayer());
			ScreenAlert->SetMessage(FText::FromString("Do you sure you want to logout ?"));
			FOnButtonClickedEvent cancelClicked;
			FOnButtonClickedEvent confirmClicked;
			confirmClicked.AddDynamic(this, &USideInfo::ConfirmLogout);
			confirmClicked.AddDynamic(ScreenAlert, &UOnScreenAlert::RemoveFromParent);
			cancelClicked.AddDynamic(ScreenAlert, &UOnScreenAlert::RemoveFromParent);
			cancelClicked.AddDynamic(this, &USideInfo::SetCustomInputMode);
			ScreenAlert->BindConfirmBtn(confirmClicked);
			ScreenAlert->BindCancelBtn(cancelClicked);
			ScreenAlert->AddToViewport(1);
		}
	}
}

void USideInfo::SetCustomInputMode() 
{
	if (APlayerController* PlayerController = this->GetOwningPlayer<APlayerController>()) {
		FInputModeGameAndUI InputMode;
		InputMode.SetHideCursorDuringCapture(false);
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PlayerController->SetInputMode(InputMode);
	}
}

void USideInfo::NextRequest()
{
	if (UMyGameInstance* GameInstance = this->GetGameInstance<UMyGameInstance>()) {
		int CurrentFriendRequestIdx = WSwitcher_FriendRequest->GetActiveWidgetIndex();
		++CurrentFriendRequestIdx;
		if (CurrentFriendRequestIdx == GameInstance->GetFriendRequestList().Num())
			CurrentFriendRequestIdx = 0;
		if (WSwitcher_FriendRequest) {
			WSwitcher_FriendRequest->SetActiveWidgetIndex(CurrentFriendRequestIdx);
		}
	}
}

void USideInfo::PrevRequest()
{
	if (UMyGameInstance* GameInstance = this->GetGameInstance<UMyGameInstance>()) {
		int CurrentFriendRequestIdx = WSwitcher_FriendRequest->GetActiveWidgetIndex();
		--CurrentFriendRequestIdx;
		if (CurrentFriendRequestIdx == -1)
			CurrentFriendRequestIdx = GameInstance->GetFriendRequestList().Num() - 1;
		if (WSwitcher_FriendRequest) {
			WSwitcher_FriendRequest->SetActiveWidgetIndex(CurrentFriendRequestIdx);
		}
	}
}

void USideInfo::SendFriendRequest()
{
	FString receiver = TxtBox_SearchUser->GetText().ToString();
	if (!receiver.IsEmpty()) {
		if (AMainMenuController* MainMenuController = this->GetOwningPlayer<AMainMenuController>()) {
			MainMenuController->SendFriendRequest(receiver);
		}
	}
}

void USideInfo::RefreshFriendlist(int CurrentOnlineNum, const TArray<FPlayerInfo>& Friendlist)
{
	if (IsValid(ScrollBox_Friendlist) && IsValid(DA_UI)) {
		int UIAndRealFriendlistNumDiff = FMath::Abs(ScrollBox_Friendlist->GetChildrenCount() - Friendlist.Num());
		if (Friendlist.Num() > ScrollBox_Friendlist->GetChildrenCount()) {
			for (int i = 0; i < UIAndRealFriendlistNumDiff; ++i) {
				CreateWidget<UFriendTag>(this->GetOwningPlayer(), *DA_UI->UISubclassMap.Find(FName("FriendTag")));
			}
			for (const FPlayerInfo& Player : Friendlist) {
				if (UFriendTag* FriendTag = CreateWidget<UFriendTag>(this->GetOwningPlayer(), *DA_UI->UISubclassMap.Find(FName("FriendTag")))) {
					FriendTag->SetUsernameText(FText::FromName(Player.Username));
					Player.isOnline ? FriendTag->SetTextOnline() : FriendTag->SetTextOffline();
					ScrollBox_Friendlist->AddChild(FriendTag);
				}
			}
		}
		else if (Friendlist.Num() < ScrollBox_Friendlist->GetChildrenCount()) {
			for (int i = 0; i < UIAndRealFriendlistNumDiff; ++i) {
				ScrollBox_Friendlist->RemoveChildAt(ScrollBox_Friendlist->GetChildrenCount() - 1);
			}
		}

		for (int i = 0; i < Friendlist.Num(); ++i) {
			if (UFriendTag* FriendTag = Cast<UFriendTag>(ScrollBox_Friendlist->GetChildAt(i))) {
				const FPlayerInfo& CurPlayerInfo = Friendlist[i];
				FriendTag->SetUsernameText(FText::FromName(CurPlayerInfo.Username));
				CurPlayerInfo.isOnline ? FriendTag->SetTextOnline() : FriendTag->SetTextOffline();
			}
		}

		SetFriendNumText(CurrentOnlineNum, Friendlist.Num());
	}
}

void USideInfo::FetchFriendRequest()
{
	if (UMyGameInstance* GameInstance = this->GetGameInstance<UMyGameInstance>()) {
		if (IsValid(WSwitcher_FriendRequest)) {
			WSwitcher_FriendRequest->ClearChildren();
			const TArray<FFriendRequest>& FriendRequestList = GameInstance->GetFriendRequestList();
			for (int i = 0; i < FriendRequestList.Num(); ++i) {
				if (UReceivedFriendRequestPanel* FriendRequestPanel = CreateWidget<UReceivedFriendRequestPanel>(this->GetOwningPlayer(), *DA_UI->UISubclassMap.Find(FName("ReceivedFriendRequest")))) {
					FriendRequestPanel->SetSenderUsername(FriendRequestList[i].Sender_Username);
					WSwitcher_FriendRequest->AddChild(FriendRequestPanel);
				}
			}
			WSwitcher_FriendRequest->SetActiveWidgetIndex(0);
		}
	}
}

void USideInfo::FetchLobbyInvitation()
{
	if (UMyGameInstance* GameInstance = this->GetGameInstance<UMyGameInstance>()) {
		if (IsValid(WSwitcher_LobbyInvitation)) {
			WSwitcher_LobbyInvitation->ClearChildren();
			const TArray<FLobbyInvitation>& LobbyInvitationList = GameInstance->GetLobbyInvitationList();
			for (int i = 0; i < LobbyInvitationList.Num(); ++i) {
				if (ULobbyInvitationPanel* LobbyInvitationPanel = CreateWidget<ULobbyInvitationPanel>(this->GetOwningPlayer(), *DA_UI->UISubclassMap.Find(FName("LobbyInvitationPanel")))) {
					LobbyInvitationPanel->SetSenderUsername(LobbyInvitationList[i].Sender_Username);
					WSwitcher_FriendRequest->AddChild(LobbyInvitationPanel);
				}
			}
			WSwitcher_LobbyInvitation->SetActiveWidgetIndex(0);
		}
	}
}

void USideInfo::RemoveReceiveFriendRequestPanel(int idx)
{
	if (IsValid(WSwitcher_FriendRequest)) {
		WSwitcher_FriendRequest->RemoveChildAt(idx);
	}
}

void USideInfo::RemoveLobbyInvitationPanel(int idx)
{
	if (IsValid(WSwitcher_LobbyInvitation)) {
		WSwitcher_LobbyInvitation->RemoveChildAt(idx);
	}
}
