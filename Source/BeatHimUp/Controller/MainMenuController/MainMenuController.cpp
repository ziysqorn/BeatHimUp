// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuController.h"
#include "../../Subsystems/UIManager/UIManagerSubsystem.h"
#include "../../Subsystems/ServiceControllerSubsystem/ServiceControllerSubsystem.h"
#include "../../CustomGameInstance/MyGameInstance.h"

void AMainMenuController::BeginPlay()
{
	Super::BeginPlay();

	GetFriendlistRequestCompleteDel.BindUObject(this, &AMainMenuController::FriendlistResponseCallback);

	if (UServiceControllerSubsystem* ServiceController = GetGameInstance()->GetSubsystem<UServiceControllerSubsystem>()) {
		ServiceController->OpenWSConnection();
		ServiceController->WSMessageReceiveDel.AddUObject(this, &AMainMenuController::OnFriendRequestReceived);
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
				ServiceController->UserAccountController->LogoutUser(MyGameInstance->GetSecretKey(), LogoutRequestCompleteDel);
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
		const FPlayerInfo* PlayerInfo = &MyGameInstance->GetPlayerInfo();
		MainMenu->SetUsernameText(FText::FromName(PlayerInfo->Username));
		if (UServiceControllerSubsystem* ServiceController = GetGameInstance()->GetSubsystem<UServiceControllerSubsystem>()) {
			//Get Friendlist every 5 sec
			GetWorld()->GetTimerManager().SetTimer(GetFriendlistTimerHandle, FTimerDelegate::CreateUObject(this,
				&AMainMenuController::RequestGetFriendlist), 5.0f, true, 0.0f);
			//Get Friend Request
			if (ServiceController->FriendlistController) {
				ServiceController->FriendlistController->GetFriendRequest(FHttpRequestCompleteDelegate::CreateUObject(this, &AMainMenuController::FriendRequestResponseCallback));
			}
		}
	}
}

void AMainMenuController::RequestGetFriendlist()
{
	if (UServiceControllerSubsystem* ServiceController = GetGameInstance()->GetSubsystem<UServiceControllerSubsystem>()) {
		if (IsValid(ServiceController->FriendlistController)) {
			ServiceController->FriendlistController->GetFriendlist(GetFriendlistRequestCompleteDel);
		}
	}
}

void AMainMenuController::OnFriendRequestReceived(const FString& Message)
{
	TSharedPtr<FJsonObject> messageObj;
	TSharedRef<TJsonReader<>> reader = TJsonReaderFactory<>::Create(Message);
	if (FJsonSerializer::Deserialize(reader, messageObj)) {
		if (messageObj.IsValid()) {
			FString resource = messageObj->GetStringField(TEXT("resource"));
			FString action = messageObj->GetStringField(TEXT("action"));
			if (resource == TEXT("friend_request") && action == TEXT("receive")) {
				TSharedPtr<FJsonObject> payloadObj = messageObj->GetObjectField(TEXT("payload"));
				if (payloadObj.IsValid()) {
					if (UMyGameInstance* MyGameInstance = GetGameInstance<UMyGameInstance>()) {
						FFriendRequest FriendRequest(FName(payloadObj->GetStringField(TEXT("sender"))), FName(payloadObj->GetStringField(TEXT("receiver"))));
						MyGameInstance->InsertFriendRequest(FriendRequest, 0);
						if (IsValid(MainMenu)) {
							MainMenu->FetchFriendRequest();
						}
					}
				}
			}
		}
	}
}

void AMainMenuController::OnSentFriendRequestComplete(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully)
{
	check(IsInGameThread());
	if (connectedSuccessfully) {
		if (IsValid(MainMenu)) {
			if (pResponse.IsValid()) {
				/*switch (pResponse->GetResponseCode()) {
				case EHttpResponseCodes::Created:
					MainMenu->DisplayOnlyCloseAlert(pResponse->GetContentAsString());
					break;
				}*/
				MainMenu->DisplayOnlyCloseAlert(pResponse->GetContentAsString());
			}
		}
	}
}

void AMainMenuController::OnAcceptFriendRequestComplete(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully)
{
	check(IsInGameThread());
	if (connectedSuccessfully) {
		if (IsValid(MainMenu)) {
			if (UMyGameInstance* MyGameInstance = GetGameInstance<UMyGameInstance>()) {
				if (pResponse.IsValid()) {
					switch (pResponse->GetResponseCode()) {
					case EHttpResponseCodes::Created:
						TSharedPtr<FJsonObject> jsonObj;
						TSharedRef<TJsonReader<>> reader = TJsonReaderFactory<>::Create(pResponse->GetContentAsString());
						if (FJsonSerializer::Deserialize(reader, jsonObj)) {
							FName sender = FName(jsonObj->GetStringField(TEXT("username")));
							bool status = jsonObj->GetBoolField(TEXT("status"));
							FPlayerInfo newFriend(sender, status);
							MyGameInstance->AddToFriendlist(newFriend);
							MainMenu->SetFriendNumText(MyGameInstance->GetCurrentOnlineFriendNum(), MyGameInstance->GetTotalFriendNum());
							MainMenu->SetupFriendlist(MyGameInstance->GetFriendlist());
							int SenderIdxInList = MyGameInstance->RemoveFriendRequestBySender(sender);
							if (SenderIdxInList >= 0) {
								MainMenu->RemoveReceiveFriendRequestPanel(SenderIdxInList);
							}
						}
						break;
					}
				}
			}
		}
	}
}

void AMainMenuController::OnDeclineFriendRequestComplete(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully)
{
	check(IsInGameThread());
	if (connectedSuccessfully) {
		if (IsValid(MainMenu)) {
			if (UMyGameInstance* MyGameInstance = GetGameInstance<UMyGameInstance>()) {
				if (pResponse.IsValid()) {
					switch (pResponse->GetResponseCode()) {
					case EHttpResponseCodes::Created:
						TSharedPtr<FJsonObject> jsonObj;
						TSharedRef<TJsonReader<>> reader = TJsonReaderFactory<>::Create(pResponse->GetContentAsString());
						if (FJsonSerializer::Deserialize(reader, jsonObj)) {
							FName sender = FName(jsonObj->GetStringField(TEXT("sender")));
							int SenderIdxInList = MyGameInstance->RemoveFriendRequestBySender(sender);
							if (SenderIdxInList >= 0) {
								MainMenu->RemoveReceiveFriendRequestPanel(SenderIdxInList);
							}
						}
						break;
					}
				}
			}
		}
	}
}

void AMainMenuController::FriendlistResponseCallback(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully)
{
	check(IsInGameThread());
	if (connectedSuccessfully) {
		if (IsValid(MainMenu)) {
			if (UMyGameInstance* MyGameInstance = GetGameInstance<UMyGameInstance>()) {
				if (pResponse.IsValid()) {
					switch (pResponse->GetResponseCode()) {
					case EHttpResponseCodes::Ok:
						TArray<TSharedPtr<FJsonValue>> jsonObjArr;
						TSharedRef<TJsonReader<>> reader = TJsonReaderFactory<>::Create(pResponse->GetContentAsString());
						if (FJsonSerializer::Deserialize(reader, jsonObjArr)) {
							MyGameInstance->RefreshFriendlist(jsonObjArr);
							MainMenu->SetFriendNumText(MyGameInstance->GetCurrentOnlineFriendNum(), MyGameInstance->GetTotalFriendNum());
							MainMenu->SetupFriendlist(MyGameInstance->GetFriendlist());
						}
						break;
					}
				}
			}
		}
	}
}

void AMainMenuController::FriendRequestResponseCallback(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully)
{
	check(IsInGameThread());
	if (connectedSuccessfully) {
		if (IsValid(MainMenu)) {
			if (UMyGameInstance* MyGameInstance = GetGameInstance<UMyGameInstance>()) {
				if (pResponse.IsValid()) {
					switch (pResponse->GetResponseCode()) {
					case EHttpResponseCodes::Ok:
						TArray<TSharedPtr<FJsonValue>> jsonObjArr;
						TSharedRef<TJsonReader<>> reader = TJsonReaderFactory<>::Create(pResponse->GetContentAsString());
						if (FJsonSerializer::Deserialize(reader, jsonObjArr)) {
							if (UMyGameInstance* GameInstance = this->GetGameInstance<UMyGameInstance>()) {
								GameInstance->RefreshFriendRequest(jsonObjArr);
							}
							if (IsValid(MainMenu)) {
								MainMenu->FetchFriendRequest();
							}
						}
						break;
					}
				}
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

void AMainMenuController::SendFriendRequest(const FString& receiver) 
{
	if (UMyGameInstance* GameInstance = Cast<UMyGameInstance>(this->GetGameInstance())) {
		if (UServiceControllerSubsystem* ServiceController = GameInstance->GetSubsystem<UServiceControllerSubsystem>()) {
			if (ServiceController->FriendlistController) {
				ServiceController->FriendlistController->SendFriendRequest(receiver, FHttpRequestCompleteDelegate::CreateUObject(this, &AMainMenuController::OnSentFriendRequestComplete));
			}
		}
	}
}

void AMainMenuController::AcceptFriendRequest(const FString& sender)
{
	if (UMyGameInstance* GameInstance = Cast<UMyGameInstance>(this->GetGameInstance())) {
		if (UServiceControllerSubsystem* ServiceController = GameInstance->GetSubsystem<UServiceControllerSubsystem>()) {
			if (ServiceController->FriendlistController) {
				ServiceController->FriendlistController->AcceptFriendRequest(sender, FHttpRequestCompleteDelegate::CreateUObject(this, &AMainMenuController::OnAcceptFriendRequestComplete));
			}
		}
	}
}

void AMainMenuController::DeclineFriendRequest(const FString& sender)
{
	if (UMyGameInstance* GameInstance = Cast<UMyGameInstance>(this->GetGameInstance())) {
		if (UServiceControllerSubsystem* ServiceController = GameInstance->GetSubsystem<UServiceControllerSubsystem>()) {
			if (ServiceController->FriendlistController) {
				ServiceController->FriendlistController->DeclineFriendRequest(sender, FHttpRequestCompleteDelegate::CreateUObject(this, &AMainMenuController::OnDeclineFriendRequestComplete));
			}
		}
	}
}
