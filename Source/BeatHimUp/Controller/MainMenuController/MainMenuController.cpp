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
		ServiceController->WSMessageReceiveDel.AddUObject(this, &AMainMenuController::OnFriendRequestAccepted);
		ServiceController->WSMessageReceiveDel.AddUObject(this, &AMainMenuController::OnBeFriendRemovedReceived);
		ServiceController->WSMessageReceiveDel.AddUObject(this, &AMainMenuController::OnLobbyInvitationReceived);
		ServiceController->WSMessageReceiveDel.AddUObject(this, &AMainMenuController::OnLobbyInvitationAcceptedReceived);
		ServiceController->WSMessageReceiveDel.AddUObject(this, &AMainMenuController::OnLobbyLeaveReceived);
		ServiceController->WSMessageReceiveDel.AddUObject(this, &AMainMenuController::OnMakeLeaderReceived);
		ServiceController->WSMessageReceiveDel.AddUObject(this, &AMainMenuController::OnBeKickFromLobbyReceived);
		ServiceController->WSMessageReceiveDel.AddUObject(this, &AMainMenuController::OnStartGameReceived);
		if (ServiceController->LobbyController) {
			ServiceController->LobbyController->CreateLobby(FHttpRequestCompleteDelegate::CreateUObject(this, &AMainMenuController::OnCreateLobbyComplete));
		}
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
	ClickEventKeys.Add(EKeys::RightMouseButton);
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

void AMainMenuController::OnFriendRequestAccepted(const FString& Message)
{
	TSharedPtr<FJsonObject> messageObj;
	TSharedRef<TJsonReader<>> reader = TJsonReaderFactory<>::Create(Message);
	if (FJsonSerializer::Deserialize(reader, messageObj)) {
		if (messageObj.IsValid()) {
			FString resource = messageObj->GetStringField(TEXT("resource"));
			FString action = messageObj->GetStringField(TEXT("action"));
			if (resource == TEXT("friend_request") && action == TEXT("accept")) {
				TSharedPtr<FJsonObject> payloadObj = messageObj->GetObjectField(TEXT("payload"));
				if (payloadObj.IsValid()) {
					if (UMyGameInstance* MyGameInstance = GetGameInstance<UMyGameInstance>()) {
						FPlayerInfo ReceivedPlayer(FName(payloadObj->GetStringField(TEXT("receiver"))), true);
						MyGameInstance->AddToFriendlist(ReceivedPlayer);
						if (IsValid(MainMenu)) {
							MainMenu->RefreshFriendlist(MyGameInstance->GetCurrentOnlineFriendNum(), MyGameInstance->GetFriendlist());
						}
					}
				}
			}
		}
	}
}

void AMainMenuController::OnBeFriendRemovedReceived(const FString& Message)
{
	TSharedPtr<FJsonObject> messageObj;
	TSharedRef<TJsonReader<>> reader = TJsonReaderFactory<>::Create(Message);
	if (FJsonSerializer::Deserialize(reader, messageObj)) {
		if (messageObj.IsValid()) {
			FString resource = messageObj->GetStringField(TEXT("resource"));
			FString action = messageObj->GetStringField(TEXT("action"));
			if (resource == TEXT("friend") && action == TEXT("removed")) {
				TSharedPtr<FJsonObject> payloadObj = messageObj->GetObjectField(TEXT("payload"));
				if (payloadObj.IsValid()) {
					if (UMyGameInstance* MyGameInstance = GetGameInstance<UMyGameInstance>()) {
						FString username = payloadObj->GetStringField(TEXT("username"));
						MyGameInstance->RemoveFromFriendlist(FName(username));
						MainMenu->RefreshFriendlist(MyGameInstance->GetCurrentOnlineFriendNum(), MyGameInstance->GetFriendlist());
					}
				}
			}
		}
	}
}

void AMainMenuController::OnLobbyInvitationReceived(const FString& Message)
{
	TSharedPtr<FJsonObject> messageObj;
	TSharedRef<TJsonReader<>> reader = TJsonReaderFactory<>::Create(Message);
	if (FJsonSerializer::Deserialize(reader, messageObj)) {
		if (messageObj.IsValid()) {
			FString resource = messageObj->GetStringField(TEXT("resource"));
			FString action = messageObj->GetStringField(TEXT("action"));
			if (resource == TEXT("lobby_invitation") && action == TEXT("receive")) {
				TSharedPtr<FJsonObject> payloadObj = messageObj->GetObjectField(TEXT("payload"));
				if (payloadObj.IsValid()) {
					if (UMyGameInstance* MyGameInstance = GetGameInstance<UMyGameInstance>()) {
						FLobbyInvitation LobbyInvitation(FName(payloadObj->GetStringField(TEXT("sender"))), FName(payloadObj->GetStringField(TEXT("receiver"))));
						MyGameInstance->InsertLobbyInvitation(LobbyInvitation, 0);
						if (IsValid(MainMenu)) {
							MainMenu->FetchLobbyInvitation();
						}
					}
				}
			}
		}
	}
}

void AMainMenuController::OnLobbyInvitationAcceptedReceived(const FString& Message)
{
	TSharedPtr<FJsonObject> messageObj;
	TSharedRef<TJsonReader<>> reader = TJsonReaderFactory<>::Create(Message);
	if (FJsonSerializer::Deserialize(reader, messageObj)) {
		if (messageObj.IsValid()) {
			FString resource = messageObj->GetStringField(TEXT("resource"));
			FString action = messageObj->GetStringField(TEXT("action"));
			if (resource == TEXT("lobby_invitation") && action == TEXT("accept")) {
				TSharedPtr<FJsonObject> payloadObj = messageObj->GetObjectField(TEXT("payload"));
				if (payloadObj.IsValid()) {
					if (UMyGameInstance* MyGameInstance = GetGameInstance<UMyGameInstance>()) {
						FString sender = payloadObj->GetStringField(TEXT("sender"));
						FString receiver = payloadObj->GetStringField(TEXT("receiver"));
						FPlayerInfo NewMember(FName(receiver), true);
						MyGameInstance->AddToLobby(NewMember);
					}
				}
			}
		}
	}
}

void AMainMenuController::OnLobbyLeaveReceived(const FString& Message)
{
	TSharedPtr<FJsonObject> messageObj;
	TSharedRef<TJsonReader<>> reader = TJsonReaderFactory<>::Create(Message);
	if (FJsonSerializer::Deserialize(reader, messageObj)) {
		if (messageObj.IsValid()) {
			FString resource = messageObj->GetStringField(TEXT("resource"));
			FString action = messageObj->GetStringField(TEXT("action"));
			if (resource == TEXT("lobby")) {
				if (action == TEXT("leave") || action == TEXT("kick_member")) {
					TSharedPtr<FJsonObject> payloadObj = messageObj->GetObjectField(TEXT("payload"));
					if (payloadObj.IsValid()) {
						TSharedPtr<FJsonObject> lobbyObj = payloadObj->GetObjectField(TEXT("lobby"));
						if (lobbyObj.IsValid()) {
							if (UMyGameInstance* MyGameInstance = GetGameInstance<UMyGameInstance>()) {
								FName LeftUsername = FName(payloadObj->GetStringField(TEXT("left_user")));
								if (!LeftUsername.IsEqual(MyGameInstance->GetLobbyInfo().Leader_Username)) {
									FString CurrentLeaderUsername = lobbyObj->GetStringField(TEXT("leader"));
									MyGameInstance->RemoveFromLobby(LeftUsername, FName(CurrentLeaderUsername));
								}
								else {
									FName LobbyName = FName(lobbyObj->GetStringField(TEXT("lobby_name")));
									FName LeaderUsername = FName(lobbyObj->GetStringField(TEXT("leader")));
									TArray<TSharedPtr<FJsonValue>> MembersJson = lobbyObj->GetArrayField(TEXT("members"));
									FString Status = lobbyObj->GetStringField(TEXT("status"));
									TArray<FPlayerInfo> LobbyMembers;
									for (int i = 0; i < MembersJson.Num(); ++i) {
										if (MembersJson[i].IsValid()) {
											FString Username = MembersJson[i]->AsString();
											FPlayerInfo Member(FName(Username), true);
											LobbyMembers.Add(Member);
										}
									}
									FLobbyInfo LobbyInfo(LobbyName, LeaderUsername, LobbyMembers, 5, Status);
									MyGameInstance->SetLobbyInfo(LobbyInfo);
								}
							}
						}
					}
				}
			}
		}
	}
}

void AMainMenuController::OnMakeLeaderReceived(const FString& Message)
{
	TSharedPtr<FJsonObject> messageObj;
	TSharedRef<TJsonReader<>> reader = TJsonReaderFactory<>::Create(Message);
	if (FJsonSerializer::Deserialize(reader, messageObj)) {
		if (messageObj.IsValid()) {
			FString resource = messageObj->GetStringField(TEXT("resource"));
			FString action = messageObj->GetStringField(TEXT("action"));
			if (resource == TEXT("lobby") && action == TEXT("make_leader")) {
				TSharedPtr<FJsonObject> payloadObj = messageObj->GetObjectField(TEXT("payload"));
				if (payloadObj.IsValid()) {
					TSharedPtr<FJsonObject> lobbyObj = payloadObj->GetObjectField(TEXT("lobby"));
					if (lobbyObj.IsValid()) {
						if (UMyGameInstance* MyGameInstance = GetGameInstance<UMyGameInstance>()) {
							FName LobbyName = FName(lobbyObj->GetStringField(TEXT("lobby_name")));
							FName LeaderUsername = FName(lobbyObj->GetStringField(TEXT("leader")));
							TArray<TSharedPtr<FJsonValue>> MembersJson = lobbyObj->GetArrayField(TEXT("members"));
							FString Status = lobbyObj->GetStringField(TEXT("status"));
							TArray<FPlayerInfo> LobbyMembers;
							for (int i = 0; i < MembersJson.Num(); ++i) {
								if (MembersJson[i].IsValid()) {
									FString Username = MembersJson[i]->AsString();
									FPlayerInfo Member(FName(Username), true);
									LobbyMembers.Add(Member);
								}
							}
							FLobbyInfo LobbyInfo(LobbyName, LeaderUsername, LobbyMembers, 5, Status);
							MyGameInstance->SetLobbyInfo(LobbyInfo);
						}
					}
				}
			}
		}
	}
}

void AMainMenuController::OnBeKickFromLobbyReceived(const FString& Message)
{
	TSharedPtr<FJsonObject> messageObj;
	TSharedRef<TJsonReader<>> reader = TJsonReaderFactory<>::Create(Message);
	if (FJsonSerializer::Deserialize(reader, messageObj)) {
		if (messageObj.IsValid()) {
			FString resource = messageObj->GetStringField(TEXT("resource"));
			FString action = messageObj->GetStringField(TEXT("action"));
			if (resource == TEXT("lobby") && action == TEXT("is_kick")) {
				TSharedPtr<FJsonObject> payloadObj = messageObj->GetObjectField(TEXT("payload"));
				if (payloadObj.IsValid()) {
					TSharedPtr<FJsonObject> lobbyObj = payloadObj->GetObjectField(TEXT("lobby"));
					if (lobbyObj.IsValid()) {
						if (UMyGameInstance* MyGameInstance = GetGameInstance<UMyGameInstance>()) {
							FName LobbyName = FName(lobbyObj->GetStringField(TEXT("lobby_name")));
							FName LeaderUsername = FName(lobbyObj->GetStringField(TEXT("leader")));
							TArray<TSharedPtr<FJsonValue>> MembersJson = lobbyObj->GetArrayField(TEXT("members"));
							FString Status = lobbyObj->GetStringField(TEXT("status"));
							TArray<FPlayerInfo> LobbyMembers;
							for (int i = 0; i < MembersJson.Num(); ++i) {
								if (MembersJson[i].IsValid()) {
									FString Username = MembersJson[i]->AsString();
									FPlayerInfo Member(FName(Username), true);
									LobbyMembers.Add(Member);
								}
							}
							FLobbyInfo LobbyInfo(LobbyName, LeaderUsername, LobbyMembers, 5, Status);
							MyGameInstance->SetLobbyInfo(LobbyInfo);
						}
					}
				}
			}
		}
	}
}

void AMainMenuController::OnStartGameReceived(const FString& Message)
{
	TSharedPtr<FJsonObject> messageObj;
	TSharedRef<TJsonReader<>> reader = TJsonReaderFactory<>::Create(Message);
	if (FJsonSerializer::Deserialize(reader, messageObj)) {
		if (messageObj.IsValid()) {
			FString resource = messageObj->GetStringField(TEXT("resource"));
			FString action = messageObj->GetStringField(TEXT("action"));
			if (resource == TEXT("game_server") && action == TEXT("create")) {
				if (IsValid(MainMenu)) {
					TSharedPtr<FJsonObject> payloadObj = messageObj->GetObjectField(TEXT("payload"));
					if (payloadObj.IsValid()) {
						TSharedPtr<FJsonObject> serverObj = payloadObj->GetObjectField(TEXT("game_server"));
						if (serverObj.IsValid()) {
							if (UMyGameInstance* MyGameInstance = GetGameInstance<UMyGameInstance>()) {
								if (UUIManagerSubsystem* UISubsystem = MyGameInstance->GetSubsystem<UUIManagerSubsystem>()) {
									UISubsystem->ShowLoadingScreen(MainMenu->GetWidgetSubclass(FName("LoadingScreen")), 10);
								}
								const FString IP = serverObj->GetStringField(TEXT("address"));
								GetWorldTimerManager().SetTimer(MoveToNewLevelHandle, FTimerDelegate::CreateUObject(this, &AMainMenuController::MoveToNewLevel, FName(IP)), MoveToNewlevelDelay, false);
							}
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
							MainMenu->RefreshFriendlist(MyGameInstance->GetCurrentOnlineFriendNum(), MyGameInstance->GetFriendlist());
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

void AMainMenuController::OnRemoveFriendComplete(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully)
{
	check(IsInGameThread());
	if (connectedSuccessfully) {
		if (IsValid(MainMenu)) {
			if (UMyGameInstance* MyGameInstance = GetGameInstance<UMyGameInstance>()) {
				if (pResponse.IsValid()) {
					switch (pResponse->GetResponseCode()) {
					case EHttpResponseCodes::Created:
						FString RemovedFriend = pResponse->GetContentAsString();
						MyGameInstance->RemoveFromFriendlist(FName(RemovedFriend));
						MainMenu->RefreshFriendlist(MyGameInstance->GetCurrentOnlineFriendNum(), MyGameInstance->GetFriendlist());
						break;
					}
				}
			}
		}
	}
}

void AMainMenuController::OnCreateLobbyComplete(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully)
{
	check(IsInGameThread());
	if (connectedSuccessfully) {
		if (IsValid(MainMenu)) {
			if (UMyGameInstance* MyGameInstance = GetGameInstance<UMyGameInstance>()) {
				if (pResponse.IsValid()) {
					switch (pResponse->GetResponseCode()) {
					case EHttpResponseCodes::Created:
						TSharedPtr<FJsonObject> LobbyJson;
						TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(pResponse->GetContentAsString());
						if (FJsonSerializer::Deserialize(Reader, LobbyJson)) {
							if (LobbyJson.IsValid()) {
								const FPlayerInfo& SelfPlayerInfo = MyGameInstance->GetPlayerInfo();
								FString LobbyName = LobbyJson->GetStringField(TEXT("lobby_name"));
								FString Status = LobbyJson->GetStringField(TEXT("status"));
								TArray<FPlayerInfo> Members;
								Members.Add(MyGameInstance->GetPlayerInfo());
								FLobbyInfo LobbyInfo(FName(LobbyName), SelfPlayerInfo.Username, Members, 5, Status);
								MyGameInstance->SetLobbyInfo(LobbyInfo);
							}
						}
						break;
					}
				}
			}
		}
	}
}

void AMainMenuController::OnInviteToLobbyComplete(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully)
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

void AMainMenuController::OnAcceptLobbyInvitationComplete(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully)
{
	check(IsInGameThread());
	if (connectedSuccessfully) {
		if (IsValid(MainMenu)) {
			if (UMyGameInstance* MyGameInstance = GetGameInstance<UMyGameInstance>()) {
				if (pResponse.IsValid()) {
					switch (pResponse->GetResponseCode()) {
					case EHttpResponseCodes::Created:
					{
						TSharedPtr<FJsonObject> JsonObj;
						TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(pResponse->GetContentAsString());
						if (FJsonSerializer::Deserialize(Reader, JsonObj)) {
							FString sender = JsonObj->GetStringField(TEXT("sender"));
							int RemovedInvitationIdx = MyGameInstance->RemoveFromLobbyInvitationList(FName(sender));
							if (RemovedInvitationIdx >= 0) {
								MainMenu->RemoveLobbyInvitationPanel(RemovedInvitationIdx);
							}
							TSharedPtr<FJsonObject> LobbyJson = JsonObj->GetObjectField(TEXT("lobby"));
							if (LobbyJson.IsValid()) {
								FString LobbyName = LobbyJson->GetStringField(TEXT("lobby_name"));
								FString Leader = LobbyJson->GetStringField(TEXT("leader"));
								TArray<TSharedPtr<FJsonValue>> MembersJson = LobbyJson->GetArrayField(TEXT("members"));
								FString Status = LobbyJson->GetStringField(TEXT("status"));
								TArray<FPlayerInfo> LobbyMembers;
								for (int i = 0; i < MembersJson.Num(); ++i) {
									if (MembersJson[i].IsValid()) {
										FString Username = MembersJson[i]->AsString();
										FPlayerInfo Member(FName(Username), true);
										LobbyMembers.Add(Member);
									}
								}
								FLobbyInfo LobbyInfo(FName(LobbyName), FName(Leader), LobbyMembers, 5, Status);
								MyGameInstance->SetLobbyInfo(LobbyInfo);
							}
						}
						break;
					}
					case EHttpResponseCodes::BadRequest:
					{
						TSharedPtr<FJsonObject> JsonObj;
						TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(pResponse->GetContentAsString());
						if (FJsonSerializer::Deserialize(Reader, JsonObj)) {
							FString sender = JsonObj->GetStringField(TEXT("sender"));
							FString message = JsonObj->GetStringField(TEXT("message"));
							int RemovedInvitationIdx = MyGameInstance->RemoveFromLobbyInvitationList(FName(sender));
							if (RemovedInvitationIdx >= 0) {
								MainMenu->RemoveLobbyInvitationPanel(RemovedInvitationIdx);
							}
							MainMenu->DisplayOnlyCloseAlert(message);
						}
						break;
					}
					default:
						MainMenu->DisplayOnlyCloseAlert(pResponse->GetContentAsString());
						break;
					}
				}
			}
		}
	}
}

void AMainMenuController::OnDeclineLobbyInvitationComplete(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully)
{
	check(IsInGameThread());
	if (connectedSuccessfully) {
		if (IsValid(MainMenu)) {
			if (UMyGameInstance* MyGameInstance = GetGameInstance<UMyGameInstance>()) {
				if (pResponse.IsValid()) {
					switch (pResponse->GetResponseCode()) {
					case EHttpResponseCodes::Created:
						int RemovedInvitationIdx = MyGameInstance->RemoveFromLobbyInvitationList(FName(pResponse->GetContentAsString()));
						if (RemovedInvitationIdx >= 0) {
							MainMenu->RemoveLobbyInvitationPanel(RemovedInvitationIdx);
						}
						break;
					}
				}
			}
		}
	}
}

void AMainMenuController::OnMakeLeaderComplete(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully)
{
	check(IsInGameThread());
	if (connectedSuccessfully) {
		if (IsValid(MainMenu)) {
			if (UMyGameInstance* MyGameInstance = GetGameInstance<UMyGameInstance>()) {
				if (pResponse.IsValid()) {
					switch (pResponse->GetResponseCode()) {
					case EHttpResponseCodes::Created:
						TSharedPtr<FJsonObject> lobbyObj;
						TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(pResponse->GetContentAsString());
						if (FJsonSerializer::Deserialize(Reader, lobbyObj)) {
							if (lobbyObj.IsValid()) {
								FName LobbyName = FName(lobbyObj->GetStringField(TEXT("lobby_name")));
								FName LeaderUsername = FName(lobbyObj->GetStringField(TEXT("leader")));
								TArray<TSharedPtr<FJsonValue>> MembersJson = lobbyObj->GetArrayField(TEXT("members"));
								FString Status = lobbyObj->GetStringField(TEXT("status"));
								TArray<FPlayerInfo> LobbyMembers;
								for (int i = 0; i < MembersJson.Num(); ++i) {
									if (MembersJson[i].IsValid()) {
										FString Username = MembersJson[i]->AsString();
										FPlayerInfo Member(FName(Username), true);
										LobbyMembers.Add(Member);
									}
								}
								FLobbyInfo LobbyInfo(LobbyName, LeaderUsername, LobbyMembers, 5, Status);
								MyGameInstance->SetLobbyInfo(LobbyInfo);
							}
						}
						break;
					}
				}
			}
		}
	}
}

void AMainMenuController::OnLeaveLobbyComplete(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully)
{
	check(IsInGameThread());
	if (connectedSuccessfully) {
		if (IsValid(MainMenu)) {
			if (UMyGameInstance* MyGameInstance = GetGameInstance<UMyGameInstance>()) {
				if (pResponse.IsValid()) {
					switch (pResponse->GetResponseCode()) {
					case EHttpResponseCodes::Created:
					{
						TSharedPtr<FJsonObject> JsonObj;
						TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(pResponse->GetContentAsString());
						if (FJsonSerializer::Deserialize(Reader, JsonObj)) {
							if (JsonObj.IsValid()) {
								FString LobbyName = JsonObj->GetStringField(TEXT("lobby_name"));
								FString Leader = JsonObj->GetStringField(TEXT("leader"));
								TArray<TSharedPtr<FJsonValue>> MembersJson = JsonObj->GetArrayField(TEXT("members"));
								FString Status = JsonObj->GetStringField(TEXT("status"));
								TArray<FPlayerInfo> LobbyMembers;
								for (int i = 0; i < MembersJson.Num(); ++i) {
									if (MembersJson[i].IsValid()) {
										FString Username = MembersJson[i]->AsString();
										FPlayerInfo Member(FName(Username), true);
										LobbyMembers.Add(Member);
									}
								}
								FLobbyInfo LobbyInfo(FName(LobbyName), FName(Leader), LobbyMembers, 5, Status);
								MyGameInstance->SetLobbyInfo(LobbyInfo);
							}
						}
						break;
					}
					default:
						MainMenu->DisplayOnlyCloseAlert(pResponse->GetContentAsString());
						break;
					}
				}
			}
		}
	}
}

void AMainMenuController::OnKickMemberFromLobbyComplete(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully)
{
	check(IsInGameThread());
	if (connectedSuccessfully) {
		if (IsValid(MainMenu)) {
			if (UMyGameInstance* MyGameInstance = GetGameInstance<UMyGameInstance>()) {
				if (pResponse.IsValid()) {
					switch (pResponse->GetResponseCode()) {
					case EHttpResponseCodes::Created:
						MainMenu->DisplayOnlyCloseAlert("Kick member successfully !");
						MyGameInstance->RemoveFromLobby(FName(pResponse->GetContentAsString()), NAME_None);
						break;
					default:
						MainMenu->DisplayOnlyCloseAlert(pResponse->GetContentAsString());
						break;
					}
				}
			}
		}
	}
}

void AMainMenuController::OnStartGameComplete(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully)
{
	check(IsInGameThread());
	if (connectedSuccessfully) {
		if (IsValid(MainMenu)) {
			if (UMyGameInstance* MyGameInstance = GetGameInstance<UMyGameInstance>()) {
				if (pResponse.IsValid()) {
					switch (pResponse->GetResponseCode()) {
					case EHttpResponseCodes::Created:
					{
						TSharedPtr<FJsonObject> jsonObj;
						TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(pResponse->GetContentAsString());
						if (FJsonSerializer::Deserialize(Reader, jsonObj)) {
							if (jsonObj.IsValid()) {
								if (UUIManagerSubsystem* UISubsystem = MyGameInstance->GetSubsystem<UUIManagerSubsystem>()) {
									UISubsystem->ShowLoadingScreen(MainMenu->GetWidgetSubclass(FName("LoadingScreen")), 10);
								}
								const FString IP = jsonObj->GetStringField(TEXT("address"));
								GetWorldTimerManager().SetTimer(MoveToNewLevelHandle, FTimerDelegate::CreateUObject(this, &AMainMenuController::MoveToNewLevel, FName(IP)), MoveToNewlevelDelay, false);
							}
						}
						break;
					}
					default:
						MainMenu->DisplayOnlyCloseAlert(pResponse->GetContentAsString());
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
							MainMenu->RefreshFriendlist(MyGameInstance->GetCurrentOnlineFriendNum(), MyGameInstance->GetFriendlist());
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

void AMainMenuController::MoveToNewLevel(FName Address)
{
	ClientTravel(Address.ToString(), ETravelType::TRAVEL_Absolute);
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

void AMainMenuController::RemoveFriend(const FString& removedFriend)
{
	if (UMyGameInstance* GameInstance = Cast<UMyGameInstance>(this->GetGameInstance())) {
		if (UServiceControllerSubsystem* ServiceController = GameInstance->GetSubsystem<UServiceControllerSubsystem>()) {
			if (ServiceController->FriendlistController) {
				ServiceController->FriendlistController->RemoveFriend(removedFriend, FHttpRequestCompleteDelegate::CreateUObject(this, &AMainMenuController::OnRemoveFriendComplete));
			}
		}
	}
}

void AMainMenuController::InviteToLobby(const FString& receiver)
{
	if (UMyGameInstance* GameInstance = Cast<UMyGameInstance>(this->GetGameInstance())) {
		if (UServiceControllerSubsystem* ServiceController = GameInstance->GetSubsystem<UServiceControllerSubsystem>()) {
			if (ServiceController->LobbyController) {
				ServiceController->LobbyController->InviteToLobby(receiver, FHttpRequestCompleteDelegate::CreateUObject(this, &AMainMenuController::OnInviteToLobbyComplete));
			}
		}
	}
}

void AMainMenuController::AcceptLobbyInvitation(const FString& sender)
{
	if (UMyGameInstance* GameInstance = Cast<UMyGameInstance>(this->GetGameInstance())) {
		if (UServiceControllerSubsystem* ServiceController = GameInstance->GetSubsystem<UServiceControllerSubsystem>()) {
			if (ServiceController->LobbyController) {
				ServiceController->LobbyController->AcceptLobbyInvitation(sender, FHttpRequestCompleteDelegate::CreateUObject(this, &AMainMenuController::OnAcceptLobbyInvitationComplete));
			}
		}
	}
}

void AMainMenuController::DeclineLobbyInvitation(const FString& sender)
{
	if (UMyGameInstance* GameInstance = Cast<UMyGameInstance>(this->GetGameInstance())) {
		if (UServiceControllerSubsystem* ServiceController = GameInstance->GetSubsystem<UServiceControllerSubsystem>()) {
			if (ServiceController->LobbyController) {
				ServiceController->LobbyController->DeclineLobbyInvitation(sender, FHttpRequestCompleteDelegate::CreateUObject(this, &AMainMenuController::OnDeclineLobbyInvitationComplete));
			}
		}
	}
}

void AMainMenuController::MakeLeader(const FString& receiver)
{
	if (UMyGameInstance* GameInstance = Cast<UMyGameInstance>(this->GetGameInstance())) {
		if (GameInstance->GetPlayerInfo().Username.IsEqual(GameInstance->GetLobbyInfo().Leader_Username)) {
			if (UServiceControllerSubsystem* ServiceController = GameInstance->GetSubsystem<UServiceControllerSubsystem>()) {
				if (ServiceController->LobbyController) {
					ServiceController->LobbyController->MakeLeader(receiver, FHttpRequestCompleteDelegate::CreateUObject(this, &AMainMenuController::OnMakeLeaderComplete));
				}
			}
		}
	}
}

void AMainMenuController::LeaveLobby()
{
	if (UMyGameInstance* GameInstance = Cast<UMyGameInstance>(this->GetGameInstance())) {
		if (UServiceControllerSubsystem* ServiceController = GameInstance->GetSubsystem<UServiceControllerSubsystem>()) {
			if (ServiceController->LobbyController) {
				ServiceController->LobbyController->LeaveLobby(FHttpRequestCompleteDelegate::CreateUObject(this, &AMainMenuController::OnLeaveLobbyComplete));
			}
		}
	}
}

void AMainMenuController::KickMemberFromLobby(const FString& receiver)
{
	if (UMyGameInstance* GameInstance = Cast<UMyGameInstance>(this->GetGameInstance())) {
		if (GameInstance->GetPlayerInfo().Username.IsEqual(GameInstance->GetLobbyInfo().Leader_Username)) {
			if (UServiceControllerSubsystem* ServiceController = GameInstance->GetSubsystem<UServiceControllerSubsystem>()) {
				if (ServiceController->LobbyController) {
					ServiceController->LobbyController->KickMemberFromLobby(receiver, FHttpRequestCompleteDelegate::CreateUObject(this, &AMainMenuController::OnKickMemberFromLobbyComplete));
				}
			}
		}
	}
}

void AMainMenuController::StartGame() 
{
	if (UMyGameInstance* GameInstance = Cast<UMyGameInstance>(this->GetGameInstance())) {
		if (UServiceControllerSubsystem* ServiceController = GameInstance->GetSubsystem<UServiceControllerSubsystem>()) {
			if (ServiceController->LobbyController) {
				ServiceController->LobbyController->StartGame(FHttpRequestCompleteDelegate::CreateUObject(this, &AMainMenuController::OnStartGameComplete));
			}
		}
	}
}
