// Fill out your copyright notice in the Description page of Project Settings.


#include "ReceivedFriendRequestPanel.h"
#include "../../Controller/MainMenuController/MainMenuController.h"

void UReceivedFriendRequestPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (IsValid(Btn_Accept))
		Btn_Accept->OnClicked.AddDynamic(this, &UReceivedFriendRequestPanel::AcceptRequest);
	if (IsValid(Btn_Decline))
		Btn_Decline->OnClicked.AddDynamic(this, &UReceivedFriendRequestPanel::DeclineRequest);
}

void UReceivedFriendRequestPanel::NativeConstruct()
{
	Super::NativeConstruct();
}

void UReceivedFriendRequestPanel::NativeDestruct()
{
	Super::NativeDestruct();
}

void UReceivedFriendRequestPanel::AcceptRequest()
{
	FString sender = Txt_Username->GetText().ToString();
	if (!sender.IsEmpty()) {
		if (AMainMenuController* MainMenuController = this->GetOwningPlayer<AMainMenuController>()) {
			MainMenuController->AcceptFriendRequest(sender);
		}
	}
}

void UReceivedFriendRequestPanel::DeclineRequest()
{
	FString sender = Txt_Username->GetText().ToString();
	if (!sender.IsEmpty()) {
		if (AMainMenuController* MainMenuController = this->GetOwningPlayer<AMainMenuController>()) {
			MainMenuController->DeclineFriendRequest(sender);
		}
	}
}
