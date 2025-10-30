// Fill out your copyright notice in the Description page of Project Settings.


#include "UIManagerSubsystem.h"


void UUIManagerSubsystem::AddWidget_Implementation(UUserWidget* inWidget)
{
	if (IsValid(inWidget)) {
		if (stk_Widgets.Num() > 0) {
			TWeakObjectPtr<UUserWidget> topWidget = stk_Widgets.Top();
			if (topWidget.IsValid()) {
				topWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
			}
		}
		TWeakObjectPtr<UUserWidget> newWidget(inWidget);
		stk_Widgets.Push(newWidget);
	}
}

void UUIManagerSubsystem::PopLastWidget_Implementation() {
	if (!stk_Widgets.IsEmpty()) {
		stk_Widgets.Pop(EAllowShrinking::No);
		if (!stk_Widgets.IsEmpty()) {
			TWeakObjectPtr<UUserWidget> topWidget = stk_Widgets.Top();
			if (topWidget.IsValid()) {
				topWidget->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
}

TWeakObjectPtr<UUserWidget>& UUIManagerSubsystem::GetTopWidget() {
	return stk_Widgets.Top();
}

void UUIManagerSubsystem::InitFriendTagCxtMenu(const TArray<TPair<FText, FOnButtonClickedEvent&>>& Options, TSubclassOf<UUserWidget> CxtMenuSubclass, FVector2D MenuPosition)
{
	if (IsValid(CtxMenu_FriendTag)) {
		int buttonCount = CtxMenu_FriendTag->GetOptionButtonCount();
		if (Options.Num() > buttonCount) {
			for (int i = 0; i < FMath::Abs(Options.Num() - buttonCount); ++i) {
				if (UButton* newButton = CtxMenu_FriendTag->WidgetTree->ConstructWidget<UButton>(UButton::StaticClass())) {
					newButton->SetCursor(EMouseCursor::Hand);
					if (UTextBlock* newTextBlock = CtxMenu_FriendTag->WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass())) {
						CtxMenu_FriendTag->AddMenuOption(newButton, newTextBlock);
					}
				}
			}
		}
		else if (Options.Num() < buttonCount) {
			for (int i = 0; i < FMath::Abs(Options.Num() - buttonCount); ++i) {
				CtxMenu_FriendTag->PopMenuOption();
			}
		}
		for (int i = 0; i < Options.Num(); ++i) {
			CtxMenu_FriendTag->ChangeMenuOption(i, Options[i].Get<0>(), Options[i].Get<1>());
		}
	}
	else {
		if (CxtMenuSubclass) {
			if (CtxMenu_FriendTag = CreateWidget<UContextMenu>(GetGameInstance()->GetFirstLocalPlayerController(), CxtMenuSubclass)) {
				for (auto& Opt : Options) {
					if (UButton* newButton = CtxMenu_FriendTag->WidgetTree->ConstructWidget<UButton>(UButton::StaticClass())) {
						newButton->SetCursor(EMouseCursor::Hand);
						newButton->OnClicked = Opt.Get<1>();
						if (UTextBlock* newTextBlock = CtxMenu_FriendTag->WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass())) {
							newTextBlock->SetText(Opt.Get<0>());
							CtxMenu_FriendTag->AddMenuOption(newButton, newTextBlock);
						}
					}
				}
			}
		}
	}
	CtxMenu_FriendTag->AddToViewport(1);
	CtxMenu_FriendTag->SetPositionInViewport(MenuPosition);
}

