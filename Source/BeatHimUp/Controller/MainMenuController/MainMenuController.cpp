// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuController.h"
#include "../../Subsystems/UIManager/UIManagerSubsystem.h"
#include "../../Subsystems/ServiceControllerSubsystem/ServiceControllerSubsystem.h"
#include "../../CustomGameInstance/MyGameInstance.h"

void AMainMenuController::BeginPlay()
{
	Super::BeginPlay();

	if (UServiceControllerSubsystem* ServiceController = GetGameInstance()->GetSubsystem<UServiceControllerSubsystem>()) {
		ServiceController->OpenWSConnection();
	}
	SetupMappingContext();
	Client_CreateMainMenu();
}

void AMainMenuController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	if (UMyGameInstance* MyGameInstance = GetGameInstance<UMyGameInstance>()) {
		if (UServiceControllerSubsystem* ServiceController = MyGameInstance->GetSubsystem<UServiceControllerSubsystem>()) {
			if (IsValid(ServiceController->UserAccountController)) {
				FHttpRequestCompleteDelegate LogoutRequestCompleteDel;
				LogoutRequestCompleteDel.BindUObject(this, &AMainMenuController::LogoutRequestComplete);
				ServiceController->UserAccountController->LogoutUser(MyGameInstance->PlayerInfo.Username.ToString(), LogoutRequestCompleteDel);
			}
		}
	}
}

void AMainMenuController::LogoutRequestComplete(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully) {
	check(IsInGameThread());
	if (connectedSuccessfully && pResponse.IsValid()) {
		switch (pResponse->GetResponseCode()) {
		case EHttpResponseCodes::Ok:
			if (UMyGameInstance* MyGameInstance = this->GetGameInstance<UMyGameInstance>()) {
				MyGameInstance->PlayerInfo.Username = NAME_None;
				MyGameInstance->PlayerInfo.isOnline = false;
				MyGameInstance->Friendlist.Empty();
			}
			break;
		}
	}
}

void AMainMenuController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void AMainMenuController::SetupMappingContext() 
{
	if (UEnhancedInputLocalPlayerSubsystem* EISubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {
		EISubsystem->AddMappingContext(InputMappingContext, 0);
	}
}


void AMainMenuController::Client_CreateMainMenu_Implementation()
{
	bEnableClickEvents = true;
	if (!IsValid(MainMenu) && MainMenuSubclass) MainMenu = CreateWidget<UMainMenu>(this, MainMenuSubclass);
	if (IsValid(MainMenu)) {
		MainMenu->SetOwningPlayer(this);
		this->SetShowMouseCursor(true);
		MainMenu->AddToViewport(0);
	}
}

void AMainMenuController::BindLeftMouseRelease(UObject* userObj, FName FuncName)
{
	if (UEnhancedInputComponent* EIComponent = Cast<UEnhancedInputComponent>(InputComponent)) {
		EIComponent->BindAction(IA_LeftMouse, ETriggerEvent::Triggered, userObj, FuncName);
	}
}

void AMainMenuController::BindLeftMouseClicked(UObject* userObj, FName FuncName)
{
	if (UEnhancedInputComponent* EIComponent = Cast<UEnhancedInputComponent>(InputComponent)) {
		EIComponent->BindAction(IA_LeftMouseClicked, ETriggerEvent::Triggered, userObj, FuncName);
	}
}