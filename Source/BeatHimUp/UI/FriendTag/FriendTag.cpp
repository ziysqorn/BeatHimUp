// Fill out your copyright notice in the Description page of Project Settings.


#include "FriendTag.h"
#include "../../Subsystems/UIManager/UIManagerSubsystem.h"

FReply UFriendTag::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton) {
		if (IsValid(DA_UI)) {
			if (UUIManagerSubsystem* UIManager = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>()) {
				FOnButtonClickedEvent InviteClick, RemoveFriendClick;
				TArray<TPair<FText, FOnButtonClickedEvent&>> Options = {
					TPair<FText, FOnButtonClickedEvent&>(FText::FromString("Invite to lobby"), InviteClick),
					TPair<FText, FOnButtonClickedEvent&>(FText::FromString("Remove friend"), RemoveFriendClick)
				};
				FVector2D MouseViewportPos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Format(TEXT("Viewport: {0}"), { MouseViewportPos.ToString()}));
				UIManager->InitFriendTagCxtMenu(Options, *DA_UI->UISubclassMap.Find("ContextMenu"), MouseViewportPos);
			}
		}
		return FReply::Handled();
	}
	return FReply::Unhandled();
}
