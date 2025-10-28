// Fill out your copyright notice in the Description page of Project Settings.


#include "FriendTag.h"

FReply UFriendTag::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton) {
		if (IsValid(DA_UI)) {
			if (UContextMenu* ContextMenu = CreateWidget<UContextMenu>(this->GetOwningPlayer(), *DA_UI->UISubclassMap.Find("ContextMenu"))) {
				TArray<FText> OptionTextes = { FText::FromString("Chat"), FText::FromString("Invite to lobby"), FText::FromString("Block") };
				for (const FText& Text : OptionTextes) {
					if (UButton* newButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass())) {
						newButton->SetCursor(EMouseCursor::Hand);
						if (UTextBlock* newTextBlock = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass())) {
							newTextBlock->SetText(Text);
							ContextMenu->AddMenuOption(newButton, newTextBlock);
						}
					}
				}
				ContextMenu->AddToViewport(1);
				ContextMenu->SetPositionInViewport(InMouseEvent.GetScreenSpacePosition());
			}
		}
		return FReply::Handled();
	}
	return FReply::Unhandled();
}
