// Fill out your copyright notice in the Description page of Project Settings.


#include "UIManagerSubsystem.h"


void UUIManagerSubsystem::AddWidget(UUserWidget* inWidget)
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

void UUIManagerSubsystem::PopLastWidget() {
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

void UUIManagerSubsystem::InitFriendTagCxtMenu(const TArray<TPair<FText, TSharedPtr<FOnButtonClickedEvent>>>& Options, TSubclassOf<UUserWidget> CxtMenuSubclass, FVector2D MenuPosition)
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
			if (FOnButtonClickedEvent* OnButtonClickEvent = Options[i].Get<1>().Get()) {
				CtxMenu_FriendTag->ChangeMenuOption(i, Options[i].Get<0>(), *OnButtonClickEvent);
			}
		}
	}
	else {
		if (CxtMenuSubclass) {
			if (CtxMenu_FriendTag = CreateWidget<UContextMenu>(GetGameInstance()->GetFirstLocalPlayerController(), CxtMenuSubclass)) {
				for (auto& Opt : Options) {
					if (UButton* newButton = CtxMenu_FriendTag->WidgetTree->ConstructWidget<UButton>(UButton::StaticClass())) {
						newButton->SetCursor(EMouseCursor::Hand);
						if (FOnButtonClickedEvent* OnButtonClickEvent = Opt.Get<1>().Get()) {
							newButton->OnClicked = *OnButtonClickEvent;
						}
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
	CtxMenu_FriendTag->SetMenuPosition(MenuPosition);
}

void UUIManagerSubsystem::ShowNonDestroyedWidget(UUserWidget* Widget, int32 ZOrder)
{
	if (IsValid(Widget)) {
		if (UGameInstance* GI = GetGameInstance()) {
			if (UGameViewportClient* GameViewportClient = GI->GetGameViewportClient()) {
				GameViewportClient->AddViewportWidgetContent(Widget->TakeWidget(), ZOrder);
			}
		}
	}
}

void UUIManagerSubsystem::RemoveNonDestroyedWidget(UUserWidget* Widget)
{
	if (IsValid(Widget)) {
		if (UGameInstance* GI = GetGameInstance()) {
			if (UGameViewportClient* GameViewportClient = GI->GetGameViewportClient()) {
				GameViewportClient->RemoveViewportWidgetContent(Widget->TakeWidget());
			}
		}
	}
}

void UUIManagerSubsystem::ShowLoadingScreen(TSubclassOf<UUserWidget> LoadingScreenSubclass, int32 ZOrder)
{
	if (UGameInstance* MyGameInstance = GetGameInstance()) {
		if (!IsValid(LoadingScreen)) {
			if (IsValid(LoadingScreenSubclass)) {
				LoadingScreen = CreateWidget<ULoadingScreen>(MyGameInstance, LoadingScreenSubclass);
			}
		}
		ShowNonDestroyedWidget(LoadingScreen, ZOrder);
	}
}

void UUIManagerSubsystem::RemoveLoadingScreen()
{
	if (IsValid(LoadingScreen)) {
		RemoveNonDestroyedWidget(LoadingScreen);
	}
}

void UUIManagerSubsystem::PostLoadMapPreparation(UWorld* InLoadedWorld)
{
	RemoveLoadingScreen();
}

void UUIManagerSubsystem::ShowUserSettingsWidget(TSubclassOf<UUserWidget> WidgetSubclass, int32 ZOrder)
{
	if (UGameInstance* MyGameInstance = GetGameInstance()) {
		if (!IsValid(UserSettingWidget)) {
			if (IsValid(WidgetSubclass)) {
				UserSettingWidget = CreateWidget<UUserSettingsWidget>(MyGameInstance, WidgetSubclass);
			}
		}
	}
	UserSettingWidget->AddToViewport(ZOrder);
}

void UUIManagerSubsystem::RemoveUserSettingsWidget()
{
	if (IsValid(UserSettingWidget)) {
		UserSettingWidget->RemoveFromParent();
	}
}

