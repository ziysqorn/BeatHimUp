// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginScreen.h"
#include "../../Subsystems/UIManager/UIManagerSubsystem.h"
#include "../../Helpers/HTTPHelper/HTTPHelper.h"

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
	if (IsValid(Btn_Login)) {
		Btn_Login->OnClicked.AddDynamic(this, &ULoginScreen::Login);
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

void ULoginScreen::Login() {
	Txt_UsernameAlert->SetText(FText::GetEmpty());
	Txt_PasswordAlert->SetText(FText::GetEmpty());
	if (CheckUsername() && CheckPassword()) {
		TSharedPtr<FJsonObject> JsonObj = MakeShareable(new FJsonObject());
		JsonObj->SetStringField(TEXT("username"), TxtBox_Username->GetText().ToString());
		JsonObj->SetStringField(TEXT("user_password"), TxtBox_Password->GetText().ToString());
		FString contentString;
		TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&contentString);
		if (FJsonSerializer::Serialize(JsonObj.ToSharedRef(), JsonWriter)) {
			if (UHTTPHelper* HTTPHelper = NewObject<UHTTPHelper>()) {
				HTTPHelper->AddHeader(TEXT("Content-Type"), TEXT("application/json"));
				if(!HTTPHelper->IsHeaderEmpty()) HTTPHelper->SendRequest("/user/login", "POST", contentString, FHttpRequestCompleteDelegate::CreateUObject(this, &ULoginScreen::LoginRequestComplete));
			}
		}
	}
}

bool ULoginScreen::CheckUsername()
{
	FRegexPattern pattern(TEXT("^[a-zA-Z0-9@]{1,12}$"));
	FRegexMatcher matcher(pattern, TxtBox_Username->GetText().ToString());
	if (!matcher.FindNext()) {
		Txt_UsernameAlert->SetText(FText::FromString("Username must not empty and below 12 characters. Must only contains a-z, A-Z, 0-9, @"));
		return false;
	}
	return true;
}

bool ULoginScreen::CheckPassword()
{
	FRegexPattern pattern(TEXT("^[a-zA-Z0-9*@]{1,12}$"));
	FRegexMatcher matcher(pattern, TxtBox_Password->GetText().ToString());
	if (!matcher.FindNext()) {
		Txt_PasswordAlert->SetText(FText::FromString("Password must not empty and below 12 characters. Must only contains a-z, A-Z, 0-9, @, *"));
		return false;
	}
	return true;
}

void ULoginScreen::LoginRequestComplete(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully)
{
	if (connectedSuccessfully && pResponse.IsValid()) {
		switch (pResponse->GetResponseCode()) {
		case 401:
			GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Green, TEXT("Wrong password !"));
			break;
		case EHttpResponseCodes::NotFound:
			GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Green, TEXT("User not found !"));
			DisplaySignUpAlert();
			break;
		default:
			GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Green, TEXT("User found !"));
			break;
		}
	}
}

void ULoginScreen::DisplaySignUpAlert_Implementation()
{
	if (!IsValid(ScreenAlert) && ScreenAlertSubclass) {
		ScreenAlert = CreateWidget<UOnScreenAlert>(this->GetOwningPlayer(), ScreenAlertSubclass);
		if (IsValid(ScreenAlert)) {
			ScreenAlert->SetOwningPlayer(this->GetOwningPlayer());
			ScreenAlert->SetMessage(FText::FromString("User doesn't exist !\nDo you wish to create a new user using the existing information ?"));
			FOnButtonClickedEvent cancelClicked;
			FOnButtonClickedEvent confirmClicked;
			cancelClicked.AddDynamic(this, &ULoginScreen::CloseScreenAlert);
			ScreenAlert->BindCancelBtn(cancelClicked);
			if (UUIManagerSubsystem* UIManager = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>()) {
				UIManager->AddWidget(ScreenAlert);
			}
			ScreenAlert->AddToViewport(1);
		}
	}
}

void ULoginScreen::CloseScreenAlert_Implementation()
{
	if (IsValid(ScreenAlert)) {
		ScreenAlert->RemoveFromParent();
		if (UUIManagerSubsystem* UIManager = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>()) {
			UIManager->PopLastWidget();
		}
	}
}

