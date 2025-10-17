// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuController.h"
#include "../../Subsystems/UIManager/UIManagerSubsystem.h"

void AMainMenuController::BeginPlay()
{
	Super::BeginPlay();

	SetupMappingContext();
	Client_DisplayLoginScreen();
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
}

void AMainMenuController::SetupAndDisplayMainMenu(const TSharedPtr<FJsonObject>& infoJsonObj)
{
	if (IsValid(MainMenu) && infoJsonObj.IsValid()) {
		if (IsValid(LoginScreen)) {
			LoginScreen->RemoveFromParent();
		}
		MainMenu->SetOwningPlayer(this);
		this->SetShowMouseCursor(true);
		MainMenu->SetUsernameText(FText::FromString(infoJsonObj->GetStringField(TEXT("username"))));
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

void AMainMenuController::Client_DisplayLoginScreen_Implementation()
{
	if (!IsValid(LoginScreen) && LoginScreenSubclass) LoginScreen = CreateWidget<ULoginScreen>(this, LoginScreenSubclass);
	if (IsValid(LoginScreen)) {
		if (IsValid(MainMenu)) {
			MainMenu->RemoveFromParent();
		}
		LoginScreen->SetOwningPlayer(this);
		this->SetShowMouseCursor(true);
		LoginScreen->AddToViewport(0);
	}
}
