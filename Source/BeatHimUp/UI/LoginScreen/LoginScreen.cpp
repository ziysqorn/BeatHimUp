// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginScreen.h"
#include "../../Subsystems/UIManager/UIManagerSubsystem.h"
#include "../../Subsystems/ServiceControllerSubsystem/ServiceControllerSubsystem.h"
#include "../../Controller/MainMenuController/MainMenuController.h"
#include "../../PlayerState/MainPlayerState.h"

void ULoginScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (IsValid(Btn_ShowPassword)) {
		Btn_ShowPassword->OnClicked.AddDynamic(this, &ULoginScreen::TogglePassword);
	}
	if (IsValid(Btn_Clear)) {
		Btn_Clear->OnClicked.AddDynamic(this, &ULoginScreen::ClearInputs);
	}
	if (IsValid(Btn_QuitGame)) {
		Btn_QuitGame->OnClicked.AddDynamic(this, &ULoginScreen::Client_ExitGame);
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

	if (UUIManagerSubsystem* UIManager = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>()) {
		UIManager->AddWidget(this);
	}
}

void ULoginScreen::NativeDestruct()
{
	Super::NativeDestruct();

	if (UUIManagerSubsystem* UIManager = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>()) {
		UIManager->PopLastWidget();
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

void ULoginScreen::Client_ExitGame_Implementation()
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
			if (UServiceControllerSubsystem* ServiceController = GetGameInstance()->GetSubsystem<UServiceControllerSubsystem>()) {
				if (ServiceController->UserAccountController) {
					ServiceController->UserAccountController->LoginUser(contentString, FHttpRequestCompleteDelegate::CreateUObject(this, &ULoginScreen::LoginRequestComplete));
				}
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

void ULoginScreen::ConfirmSignup()
{
	TSharedPtr<FJsonObject> JsonObj = MakeShareable(new FJsonObject());
	JsonObj->SetStringField(TEXT("username"), TxtBox_Username->GetText().ToString());
	JsonObj->SetStringField(TEXT("user_password"), TxtBox_Password->GetText().ToString());
	FString contentString;
	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&contentString);
	if (FJsonSerializer::Serialize(JsonObj.ToSharedRef(), JsonWriter)) {
		if (UServiceControllerSubsystem* ServiceController = GetGameInstance()->GetSubsystem<UServiceControllerSubsystem>()) {
			if (ServiceController->UserAccountController) {
				ServiceController->UserAccountController->CreateUser(contentString, FHttpRequestCompleteDelegate::CreateUObject(this, &ULoginScreen::SignupRequestComplete));
			}
		}
	}
}

void ULoginScreen::Client_DisplayOnlyCloseAlert_Implementation(const FString& Message)
{
	if (OnlyCloseAlertSubclass) {
		if (UOnlyCloseAlert* OnlyCloseAlert = CreateWidget<UOnlyCloseAlert>(this->GetOwningPlayer(), OnlyCloseAlertSubclass)) {
			OnlyCloseAlert->SetOwningPlayer(this->GetOwningPlayer());
			OnlyCloseAlert->SetMessage(FText::FromString(Message));
			OnlyCloseAlert->AddToViewport(2);
		}
	}
}

void ULoginScreen::LoginRequestComplete(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully)
{
	if (connectedSuccessfully && pResponse.IsValid()) {
		switch (pResponse->GetResponseCode()) {
		case 401:
			Client_DisplayOnlyCloseAlert("Wrong password !");
			break;
		case EHttpResponseCodes::NotFound:
			Client_DisplaySignUpAlert();
			break;
		default:
			if (AMainMenuController* MainMenuController = this->GetOwningPlayer<AMainMenuController>()) {
				TSharedPtr<FJsonObject> JsonObj = MakeShareable(new FJsonObject());
				TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(pResponse->GetContentAsString());
				if (FJsonSerializer::Deserialize(Reader, JsonObj)) {
					if (AMainPlayerState* MainPlayerState = MainMenuController->GetPlayerState<AMainPlayerState>()) {
						MainPlayerState->SetUsername(FName(JsonObj->GetStringField("username")));
					}
				}
				MainMenuController->Client_DisplayMainMenu();
			}
			break;
		}
	}
}

void ULoginScreen::SignupRequestComplete(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully)
{
	if (connectedSuccessfully && pResponse.IsValid()) {
		switch (pResponse->GetResponseCode()) {
		case EHttpResponseCodes::Created:
			Client_CloseScreenAlert();
			Login();
			break;
		case EHttpResponseCodes::Conflict:
			Client_DisplayOnlyCloseAlert(pResponse->GetContentAsString());
			break;
		default:
			Client_DisplayOnlyCloseAlert("Invalid inputs !");
			break;
		}
	}
}

void ULoginScreen::Client_DisplaySignUpAlert_Implementation()
{
	if (ScreenAlertSubclass) {
		if (UOnScreenAlert* ScreenAlert = CreateWidget<UOnScreenAlert>(this->GetOwningPlayer(), ScreenAlertSubclass)) {
			ScreenAlert->SetOwningPlayer(this->GetOwningPlayer());
			ScreenAlert->SetMessage(FText::FromString("User doesn't exist !\nDo you wish to create a new user using the existing information ?"));
			FOnButtonClickedEvent cancelClicked;
			FOnButtonClickedEvent confirmClicked;
			confirmClicked.AddDynamic(this, &ULoginScreen::ConfirmSignup);
			cancelClicked.AddDynamic(this, &ULoginScreen::Client_CloseScreenAlert);
			ScreenAlert->BindConfirmBtn(confirmClicked);
			ScreenAlert->BindCancelBtn(cancelClicked);
			ScreenAlert->AddToViewport(1);
		}
	}
}

void ULoginScreen::Client_CloseScreenAlert_Implementation()
{
	if (UUIManagerSubsystem* UIManager = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>()) {
		TWeakObjectPtr<UUserWidget> ScreenAlert = UIManager->GetTopWidget();
		if (ScreenAlert.IsValid()) ScreenAlert->RemoveFromParent();
	}
}

