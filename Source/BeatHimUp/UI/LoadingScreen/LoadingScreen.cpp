// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingScreen.h"
#include "../../Subsystems/UIManager/UIManagerSubsystem.h"
#include "../../Interface/HaveSpecialInputMode.h"

void ULoadingScreen::NativeConstruct()
{
	Super::NativeConstruct();

	if (UUIManagerSubsystem* UIManager = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>()) {
		UIManager->AddWidget(this);
		if (APlayerController* PC = GetOwningPlayer()) {
			FInputModeUIOnly InputMode;
			PC->SetInputMode(InputMode);
		}
	}
}

void ULoadingScreen::NativeDestruct()
{
	if (UUIManagerSubsystem* UIManager = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>()) {
		UIManager->PopLastWidget();
		if (IHaveSpecialInputMode* HaveSpecialInputMode = GetOwningPlayer<IHaveSpecialInputMode>()) {
			HaveSpecialInputMode->HandleAfterUIRemove();
		}
	}
	Super::NativeDestruct();
}
