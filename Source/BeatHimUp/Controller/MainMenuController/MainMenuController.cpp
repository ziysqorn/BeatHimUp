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
				LogoutRequestCompleteDel.BindUObject(MyGameInstance, &UMyGameInstance::LogoutRequestComplete);
				ServiceController->UserAccountController->LogoutUser(MyGameInstance->PlayerInfo.Username.ToString(), LogoutRequestCompleteDel);
			}
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