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
	
	if (GetWorld()->GetTimerManager().IsTimerActive(GetFriendlistTimerHandle)) {
		GetWorld()->GetTimerManager().ClearTimer(GetFriendlistTimerHandle);
	}
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
		InitForMainMenu();
	}
}

void AMainMenuController::InitForMainMenu()
{
	this->SetInputMode(FInputModeUIOnly());
	if (UMyGameInstance* MyGameInstance = this->GetGameInstance<UMyGameInstance>()) {
		MainMenu->SetUsernameText(FText::FromName(MyGameInstance->PlayerInfo.Username));
		if (UServiceControllerSubsystem* ServiceController = GetGameInstance()->GetSubsystem<UServiceControllerSubsystem>()) {
			ServiceController->WSMessageRecieveDel.AddUObject(this, &AMainMenuController::FriendlistMessageRecvCallback);
			GetWorld()->GetTimerManager().SetTimer(GetFriendlistTimerHandle, FTimerDelegate::CreateUObject(ServiceController->FriendlistController,
				&UFriendlistController::SendFriendlistMessage, MyGameInstance->PlayerInfo.Username), 5.0f, true, 0.0f);
		}
	}
}

void AMainMenuController::FriendlistMessageRecvCallback_Implementation(const FString& Message)
{
	if (IsValid(MainMenu)) {
		if (UMyGameInstance* MyGameInstance = GetGameInstance<UMyGameInstance>()) {
			TArray<TSharedPtr<FJsonValue>> jsonObjArr;
			TSharedRef<TJsonReader<>> reader = TJsonReaderFactory<>::Create(Message);
			if (FJsonSerializer::Deserialize(reader, jsonObjArr)) {
				int onlineNum = 0;
				MyGameInstance->Friendlist.Empty();
				for (const TSharedPtr<FJsonValue>& Value : jsonObjArr) {
					TSharedPtr<FJsonObject> friendJsonObj = Value->AsObject();
					if (friendJsonObj.IsValid()) {
						FPlayerInfo newPlayerInfo;
						newPlayerInfo.Username = FName(friendJsonObj->GetStringField(TEXT("username")));
						newPlayerInfo.isOnline = friendJsonObj->GetBoolField(TEXT("status"));
						if (newPlayerInfo.isOnline) ++onlineNum;
						MyGameInstance->Friendlist.Add(newPlayerInfo);
					}
				}
				MainMenu->SetFriendNumText(onlineNum, jsonObjArr.Num());
				MyGameInstance->Friendlist.Sort([](const FPlayerInfo& player1, const FPlayerInfo& player2) {
					return player1.isOnline && !player2.isOnline;
					});
				MainMenu->SetupFriendlist(MyGameInstance->Friendlist);
			}
		}
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