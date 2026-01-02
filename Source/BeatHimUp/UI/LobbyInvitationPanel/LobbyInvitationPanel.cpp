// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyInvitationPanel.h"
#include "../../Controller/MainMenuController/MainMenuController.h"

void ULobbyInvitationPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (IsValid(Btn_Accept))
		Btn_Accept->OnClicked.AddDynamic(this, &ULobbyInvitationPanel::AcceptInvitation);
	if (IsValid(Btn_Decline))
		Btn_Decline->OnClicked.AddDynamic(this, &ULobbyInvitationPanel::DeclineInvitation);
}

void ULobbyInvitationPanel::NativeConstruct()
{
	Super::NativeConstruct();
}

void ULobbyInvitationPanel::NativeDestruct()
{
	Super::NativeDestruct();
}

void ULobbyInvitationPanel::AcceptInvitation()
{
}

void ULobbyInvitationPanel::DeclineInvitation()
{
	FString sender = Txt_Username->GetText().ToString();
	if (!sender.IsEmpty()) {
		if (AMainMenuController* MainMenuController = this->GetOwningPlayer<AMainMenuController>()) {
			MainMenuController->DeclineFriendRequest(sender);
		}
	}
}

