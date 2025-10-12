// Fill out your copyright notice in the Description page of Project Settings.


#include "OnlyCloseAlert.h"
#include "../../Subsystems/UIManager/UIManagerSubsystem.h"

void UOnlyCloseAlert::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UOnlyCloseAlert::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(Btn_Confirm)) Btn_Confirm->OnClicked.AddDynamic(this, &UOnlyCloseAlert::CloseAlert);

	if (UUIManagerSubsystem* UIManager = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>()) {
		UIManager->AddWidget(this);
	}
}

void UOnlyCloseAlert::NativeDestruct()
{
	Super::NativeDestruct();

	if (IsValid(Btn_Confirm)) Btn_Confirm->OnClicked.Clear();

	if (UUIManagerSubsystem* UIManager = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>()) {
		UIManager->PopLastWidget();
	}
}

void UOnlyCloseAlert::CloseAlert_Implementation()
{
	RemoveFromParent();
}
