// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginScreen.h"

void ULoginScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (IsValid(Btn_ShowPassword)) {
		Btn_ShowPassword->SetCursor(EMouseCursor::Hand);
		Btn_ShowPassword->OnClicked.AddDynamic(this, &ULoginScreen::TogglePassword);
	}
	if (IsValid(Btn_Clear)) {
		Btn_Clear->OnClicked.AddDynamic(this, &ULoginScreen::ClearInputs);
	}
	if (IsValid(Btn_QuitGame)) {
		Btn_QuitGame->OnClicked.AddDynamic(this, &ULoginScreen::ExitGame);
	}
}

void ULoginScreen::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(TxtBox_Password)) {
		if (IsValid(Btn_ShowPassword)) {
			FButtonStyle style = Btn_ShowPassword->GetStyle();
			FSlateBrush brush = style.Normal;
			brush.SetResourceObject(EyeArr[TxtBox_Password->GetIsPassword()]);
			style.SetNormal(brush);
			style.SetHovered(brush);
			style.SetPressed(brush);
			Btn_ShowPassword->SetStyle(style);
		}
	}
}

void ULoginScreen::TogglePassword()
{
	if (IsValid(TxtBox_Password)) {
		bool passwordHidden = TxtBox_Password->GetIsPassword();
		TxtBox_Password->SetIsPassword(!passwordHidden);
		if (IsValid(Btn_ShowPassword)) {
			FButtonStyle style = Btn_ShowPassword->GetStyle();
			FSlateBrush brush = style.Normal;
			brush.SetResourceObject(EyeArr[TxtBox_Password->GetIsPassword()]);
			style.SetNormal(brush);
			style.SetHovered(brush);
			style.SetPressed(brush);
			Btn_ShowPassword->SetStyle(style);
		}
	}
}

void ULoginScreen::DisplaySignUp()
{

}

void ULoginScreen::ClearInputs() {
	if (IsValid(TxtBox_Username)) {
		TxtBox_Username->SetText(FText::GetEmpty());
	}
	if (IsValid(TxtBox_Password)) {
		TxtBox_Password->SetText(FText::GetEmpty());
	}
}

void ULoginScreen::ExitGame_Implementation()
{
	UKismetSystemLibrary::QuitGame(this, this->GetOwningPlayer(), EQuitPreference::Quit, false);
}

