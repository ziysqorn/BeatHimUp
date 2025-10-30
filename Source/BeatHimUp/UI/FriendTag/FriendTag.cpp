// Fill out your copyright notice in the Description page of Project Settings.


#include "FriendTag.h"
#include "../../Subsystems/UIManager/UIManagerSubsystem.h"

FReply UFriendTag::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton) {
		if (IsValid(DA_UI)) {
			if (UUIManagerSubsystem* UIManager = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>()) {
				FOnButtonClickedEvent ChatClick, InviteClick, BlockClick;
				TArray<TPair<FText, FOnButtonClickedEvent&>> Options = {
					TPair<FText, FOnButtonClickedEvent&>(FText::FromString("Chat"), ChatClick),
					TPair<FText, FOnButtonClickedEvent&>(FText::FromString("Invite to lobby"), InviteClick),
					TPair<FText, FOnButtonClickedEvent&>(FText::FromString("Block"), BlockClick)
				};
				UIManager->InitFriendTagCxtMenu(Options, *DA_UI->UISubclassMap.Find("ContextMenu"), InMouseEvent.GetScreenSpacePosition());
			}
		}
		return FReply::Handled();
	}
	return FReply::Unhandled();
}
