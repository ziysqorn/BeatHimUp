// Fill out your copyright notice in the Description page of Project Settings.


#include "ContextMenu.h"

void UContextMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UContextMenu::NativeConstruct()
{
	Super::NativeConstruct();
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
