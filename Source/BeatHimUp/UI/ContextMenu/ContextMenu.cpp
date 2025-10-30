// Fill out your copyright notice in the Description page of Project Settings.


#include "ContextMenu.h"
#include "../../Controller/MainMenuController/MainMenuController.h"

void UContextMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UContextMenu::NativeConstruct()
{
	Super::NativeConstruct();

	if (AMainMenuController* MainMenuController = this->GetOwningPlayer<AMainMenuController>()) {
		MainMenuController->BindLeftMouseClicked(this, FName("RemoveFromParent"));
	}
}

void UContextMenu::NativeDestruct()
{
	Super::NativeDestruct();
}

void UContextMenu::AddMenuOption(UButton* inButton, UTextBlock* inTextBlock)
{
	if (IsValid(inButton) && IsValid(inTextBlock)) {
		if (IsValid(VerBox_Menu)) {
			FSlateColor textColor(FColor::Black);
			inButton->SetStyle(ButtonStyle);
			inTextBlock->SetFont(FontStyle);
			inTextBlock->SetColorAndOpacity(textColor);
			inTextBlock->SetMargin(FMargin(0.0f, 5.0f));
			inButton->AddChild(inTextBlock);
			VerBox_Menu->AddChildToVerticalBox(inButton);
		}
	}
}

void UContextMenu::ChangeMenuOption(int32 idx, FText newOptionText, FOnButtonClickedEvent& ClickedEvent)
{
	if (IsValid(VerBox_Menu)) {
		if (UButton* OptionButton = Cast<UButton>(VerBox_Menu->GetChildAt(idx))) {
			OptionButton->OnClicked = ClickedEvent;
			if (UTextBlock* OptionText = Cast<UTextBlock>(OptionButton->GetChildAt(0))) {
				OptionText->SetText(newOptionText);
			}
		}
	}
}
