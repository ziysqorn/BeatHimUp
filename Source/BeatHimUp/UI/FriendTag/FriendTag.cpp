// Fill out your copyright notice in the Description page of Project Settings.


#include "FriendTag.h"
#include "../../Subsystems/UIManager/UIManagerSubsystem.h"
#include "../../Controller/MainMenuController/MainMenuController.h"

FReply UFriendTag::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton) {
		if (IsValid(DA_UI)) {
			if (UUIManagerSubsystem* UIManager = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>()) {
				if (IsValid(Txt_OnlineStatus)) {
					FString OnlineStatusText = Txt_OnlineStatus->GetText().ToString();
					TArray<TPair<FText, TSharedPtr<FOnButtonClickedEvent>>> Options;
					if (OnlineStatusText == "Online") {
						TSharedPtr<FOnButtonClickedEvent> SharedPtr_InviteClick = MakeShared<FOnButtonClickedEvent>();
						TSharedPtr<FOnButtonClickedEvent> SharedPtr_RemoveFriendClick = MakeShared<FOnButtonClickedEvent>();
						SharedPtr_InviteClick->AddDynamic(this, &UFriendTag::InviteToLobby);
						SharedPtr_RemoveFriendClick->AddDynamic(this, &UFriendTag::RemoveFriend);
						Options.Add(TPair<FText, TSharedPtr<FOnButtonClickedEvent>>(FText::FromString("Invite to lobby"), SharedPtr_InviteClick));
						Options.Add(TPair<FText, TSharedPtr<FOnButtonClickedEvent>>(FText::FromString("Remove friend"), SharedPtr_RemoveFriendClick));
					}
					else {
						TSharedPtr<FOnButtonClickedEvent> SharedPtr_RemoveFriendClick = MakeShared<FOnButtonClickedEvent>();
						SharedPtr_RemoveFriendClick->AddDynamic(this, &UFriendTag::RemoveFriend);
						Options.Add(TPair<FText, TSharedPtr<FOnButtonClickedEvent>>(FText::FromString("Remove friend"), SharedPtr_RemoveFriendClick));
					}
					FVector2D MouseViewportPos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
					UIManager->InitFriendTagCxtMenu(Options, *DA_UI->UISubclassMap.Find("ContextMenu"), MouseViewportPos);
				}
			}
		}
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

void UFriendTag::InviteToLobby()
{
	if (IsValid(Txt_Username)) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Format(TEXT("Invite user {0} to lobby"), { Txt_Username->GetText().ToString() }));
		FString InvitedUsername = Txt_Username->GetText().ToString();
		if (AMainMenuController* MainMenuController = this->GetOwningPlayer<AMainMenuController>()) {
			MainMenuController->InviteToLobby(InvitedUsername);
		}
		if (UUIManagerSubsystem* UIManager = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>()) {
			UIManager->HideCtxMenu();
		}
	}
}

void UFriendTag::RemoveFriend()
{
	if (IsValid(Txt_Username)) {
		FString RemovedFriendUsername = Txt_Username->GetText().ToString();
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Format(TEXT("Remove friend user {0}"), { RemovedFriendUsername }));
		if (UGameInstance* GameInstance = GetGameInstance()) {
			if (AMainMenuController* MainMenuController = this->GetOwningPlayer<AMainMenuController>()) {
				MainMenuController->RemoveFriend(RemovedFriendUsername);
				if (UUIManagerSubsystem* UIManager = GameInstance->GetSubsystem<UUIManagerSubsystem>()) {
					UIManager->HideCtxMenu();
				}
			}
		}
	}
}
