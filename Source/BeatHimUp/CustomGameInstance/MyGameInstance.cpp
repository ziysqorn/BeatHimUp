// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "../Subsystems/ServiceControllerSubsystem/ServiceControllerSubsystem.h"
#include "../Subsystems/UIManager/UIManagerSubsystem.h"
#include "../Controller/MainController/MainController.h"

void UMyGameInstance::Init()
{
	Super::Init();

	FCoreDelegates::OnHandleSystemError.AddUObject(this, &UMyGameInstance::CrashHandle);

	if (!IsDedicatedServerInstance()) {
		if (UServiceControllerSubsystem* ServiceControllerSubsystem = this->GetSubsystem<UServiceControllerSubsystem>()) {
			ServiceControllerSubsystem->WSMessageReceiveDel.AddUObject(this, &UMyGameInstance::OnLobbyInvitationAcceptedReceived);
			ServiceControllerSubsystem->WSMessageReceiveDel.AddUObject(this, &UMyGameInstance::OnLobbyLeaveReceived);
			ServiceControllerSubsystem->WSMessageReceiveDel.AddUObject(this, &UMyGameInstance::OnMakeLeaderReceived);
			ServiceControllerSubsystem->WSMessageReceiveDel.AddUObject(this, &UMyGameInstance::OnBeKickFromLobbyReceived);
		}
	}

	if (UUIManagerSubsystem* UISubs = GetSubsystem<UUIManagerSubsystem>()) {
		FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(UISubs, &UUIManagerSubsystem::PostLoadMapPreparation);
	}

	ApplySavedGraphicSettings();

	GetDotgIP(FHttpRequestCompleteDelegate::CreateUObject(this, &UMyGameInstance::OnGetDotgIPComplete));
}

void UMyGameInstance::Shutdown()
{
	bMustShutdown = true;
	LogoutProcess();
	Super::Shutdown();
}

void UMyGameInstance::ApplySavedGraphicSettings()
{
	if (GEngine) {
		if (UGameUserSettings* GameUserSettings = GEngine->GetGameUserSettings()) {
			GameUserSettings->GetOverallScalabilityLevel() > -1 ? 
				GameUserSettings->SetOverallScalabilityLevel(GameUserSettings->GetOverallScalabilityLevel()) : GameUserSettings->SetOverallScalabilityLevel(0);
			GameUserSettings->GetFrameRateLimit() > 0.0f ? 
				GameUserSettings->SetFrameRateLimit(GameUserSettings->GetFrameRateLimit()) : GameUserSettings->SetFrameRateLimit(60.f);
			GameUserSettings->SetVSyncEnabled(GameUserSettings->IsVSyncEnabled());
			auto* ConsoleVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.AntiAliasingMethod"));
			if (ConsoleVar)
			{
				ConsoleVar->Set(2);
			}
			GameUserSettings->SetAntiAliasingQuality(2);
			GameUserSettings->ApplySettings(false);
		}
	}
}

void UMyGameInstance::OnGetDotgIPComplete(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully)
{
	check(IsInGameThread());

	if (connectedSuccessfully) {
		if (pResponse.IsValid()) {
			if (UServiceControllerSubsystem* ServiceControllerSubsystem = this->GetSubsystem<UServiceControllerSubsystem>()) {
				switch (pResponse->GetResponseCode()) {
				case EHttpResponseCodes::Ok:
					FString API_IP = pResponse->GetContentAsString();
					ServiceControllerSubsystem->SetBaseAPIURL(API_IP.Append(TEXT(":3000")));
					break;
				}
			}
		}
	}
}

void UMyGameInstance::GetDotgIP(const FHttpRequestCompleteDelegate& callback)
{
	FString URL = TEXT("https://dotg.tranduyquan2003.workers.dev/api/get-dotg-ip");
	FHttpModule& HttpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest> httpRequest = HttpModule.CreateRequest();
	httpRequest->SetVerb("GET");
	httpRequest->SetURL(URL);
	httpRequest->OnProcessRequestComplete() = callback;
	httpRequest->ProcessRequest();
}

void UMyGameInstance::LogoutProcess()
{
	if (UServiceControllerSubsystem* ServiceController = this->GetSubsystem<UServiceControllerSubsystem>()) {
		//if (ServiceController->LobbyController) {
		//	ServiceController->LobbyController->LeaveLobby(FHttpRequestCompleteDelegate());
		//}
		if (IsValid(ServiceController->UserAccountController)) {
			FHttpRequestCompleteDelegate LogoutRequestCompleteDel;
			LogoutRequestCompleteDel.BindUObject(this, &UMyGameInstance::LogoutRequestComplete);
			ServiceController->UserAccountController->LogoutUser(this->SecretToken, LogoutRequestCompleteDel);
			if (bMustShutdown) {
				ServiceController->CloseWSConnection();
			}
		}
	}
}

void UMyGameInstance::CrashHandle()
{
	bMustShutdown = true;
	LogoutProcess();
}

void UMyGameInstance::LogoutRequestComplete(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully) {
	check(IsInGameThread());
	if (connectedSuccessfully && pResponse.IsValid()) {
		switch (pResponse->GetResponseCode()) {
		case EHttpResponseCodes::Ok:
			if (UServiceControllerSubsystem* ServiceController = this->GetSubsystem<UServiceControllerSubsystem>()) {
				ClearClientInfo();
				ServiceController->CloseWSConnection();
			}
			break;
		}
	}
}

void UMyGameInstance::AddToFriendlist(const FPlayerInfo& inPlayer)
{
	if (inPlayer.isOnline)
		CurrentOnlineFriendNum += 1;
	Friendlist.Add(inPlayer);
	Friendlist.Sort([](const FPlayerInfo& player1, const FPlayerInfo& player2) {
		return player1.isOnline && !player2.isOnline;
		});
	for (int i = 0; i < Friendlist.Num(); ++i) {
		if (Friendlist[i].Username.IsEqual(inPlayer.Username)) {
			FriendlistIdxMap.Add(inPlayer.Username, i);
			return;
		}
	}
}

int UMyGameInstance::RemoveFromFriendlist(FName target)
{
	if (int* result = FriendlistIdxMap.Find(target)) {
		int idx = *result;
		if (Friendlist[idx].isOnline)
			CurrentOnlineFriendNum -= 1;
		FriendlistIdxMap.Remove(target);
		Friendlist.RemoveAt(idx, EAllowShrinking::No);
		return idx;
	}
	return -1;
}

void UMyGameInstance::RefreshFriendRequest(const TArray<TSharedPtr<FJsonValue>>& jsonObjArr)
{
	int diff = FMath::Abs(jsonObjArr.Num() - FriendRequestList.Num());
	if (jsonObjArr.Num() > FriendRequestList.Num()) {
		for (int i = 0; i < diff; ++i) {
			FriendRequestList.Add(FFriendRequest());
		}
	}
	else {
		for (int i = 0; i < diff; ++i) {
			FriendRequestList.Pop(EAllowShrinking::No);
		}
	}
	for (int i = 0; i < jsonObjArr.Num(); ++i) {
		TSharedPtr<FJsonObject> friendRequestObj = jsonObjArr[i]->AsObject();
		FString sender = friendRequestObj->GetStringField(TEXT("sender"));
		FString receiver = friendRequestObj->GetStringField(TEXT("receiver"));
		FriendRequestList[i].Sender_Username = FName(sender);
		FriendRequestList[i].Receiver_Username = FName(receiver);
	}
}

void UMyGameInstance::RefreshFriendlist(const TArray<TSharedPtr<FJsonValue>>& jsonObjArr)
{
	FriendlistIdxMap.Empty();
	CurrentOnlineFriendNum = 0;
	int diff = FMath::Abs(jsonObjArr.Num() - Friendlist.Num());
	if (jsonObjArr.Num() > Friendlist.Num()) {
		for (int i = 0; i < diff; ++i) {
			Friendlist.Add(FPlayerInfo());
		}
	}
	else {
		for (int i = 0; i < diff; ++i) {
			Friendlist.Pop(EAllowShrinking::No);
		}
	}
	for (int i = 0; i < jsonObjArr.Num(); ++i) {
		TSharedPtr<FJsonObject> friendJsonObj = jsonObjArr[i]->AsObject();
		if (friendJsonObj.IsValid()) {
			Friendlist[i].Username = FName(friendJsonObj->GetStringField(TEXT("username")));
			Friendlist[i].isOnline = friendJsonObj->GetBoolField(TEXT("status"));
			if (Friendlist[i].isOnline) ++CurrentOnlineFriendNum;
		}
	}
	Friendlist.Sort([](const FPlayerInfo& player1, const FPlayerInfo& player2) {
		return player1.isOnline && !player2.isOnline;
		});
	for (int i = 0; i < Friendlist.Num(); ++i) {
		FriendlistIdxMap.Add(Friendlist[i].Username, i);
	}
}

void UMyGameInstance::InsertLobbyInvitation(const FLobbyInvitation& inInvitation, int idx)
{
	if (!LobbyInvitationIdxMap.Contains(inInvitation.Sender_Username)) {
		LobbyInvitationList.Insert(inInvitation, idx);
		LobbyInvitationIdxMap.Add(inInvitation.Sender_Username, idx);
	}
}

int UMyGameInstance::RemoveFromLobbyInvitationList(FName target)
{
	if (int* result = LobbyInvitationIdxMap.Find(target)) {
		int idx = *result;
		LobbyInvitationIdxMap.Remove(target);
		LobbyInvitationList.RemoveAt(idx, EAllowShrinking::No);
		return idx;
	}
	return -1;
}

void UMyGameInstance::AddToLobby(const FPlayerInfo& NewMember)
{
	if (LobbyInfo.Members.Num() < LobbyInfo.MaxMember) {
		for (const FPlayerInfo& Member : LobbyInfo.Members) {
			if (Member.Username.IsEqual(NewMember.Username)) return;
		}
		LobbyInfo.Members.Add(NewMember);
		if (OnLobbyUpdateDel.IsBound()) {
			OnLobbyUpdateDel.Broadcast(LobbyInfo);
		}
	}
}

int UMyGameInstance::RemoveFromLobby(FName UsernameToRemove, FName LeaderUsername)
{
	for (int i = 0; i < LobbyInfo.Members.Num(); ++i) {
		if (LobbyInfo.Members[i].Username.IsEqual(UsernameToRemove)) {
			if(!LeaderUsername.IsNone())
				LobbyInfo.Leader_Username = LeaderUsername;
			if (!LobbyInfo.Leader_Username.IsEqual(LobbyInfo.Members[i].Username)) {
				LobbyInfo.Members.RemoveAt(i, EAllowShrinking::No);
				if (OnLobbyUpdateDel.IsBound()) {
					OnLobbyUpdateDel.Broadcast(LobbyInfo);
				}
				return i;
			}
		}
	}
	return -1;
}

void UMyGameInstance::ClearClientInfo()
{
	SecretToken = TEXT("");
	PlayerInfo.Username = NAME_None;
	PlayerInfo.isOnline = false;
	LobbyInfo.LobbyName = NAME_None;
	LobbyInfo.Leader_Username = NAME_None;
	LobbyInfo.Members.Empty();
	Friendlist.Empty();
	FriendRequestList.Empty();
	LobbyInvitationList.Empty();
	FriendlistIdxMap.Empty();
	LobbyInvitationIdxMap.Empty();
}

void UMyGameInstance::OnPlayerJoinLobby(const FString& Message)
{
	TSharedPtr<FJsonObject> messageObj;
	TSharedRef<TJsonReader<>> reader = TJsonReaderFactory<>::Create(Message);
	if (FJsonSerializer::Deserialize(reader, messageObj)) {
		if (messageObj.IsValid()) {
			FString resource = messageObj->GetStringField(TEXT("resource"));
			FString action = messageObj->GetStringField(TEXT("action"));
			if (resource == TEXT("lobby") && action == TEXT("player_join")) {
				TSharedPtr<FJsonObject> payloadObj = messageObj->GetObjectField(TEXT("payload"));
				if (payloadObj.IsValid()) {
					FString NewPlayerUsername = payloadObj->GetStringField(TEXT("username"));
					FPlayerInfo NewMember(FName(NewPlayerUsername), true);
					AddToLobby(NewMember);
				}
			}
		}
	}
}

void UMyGameInstance::OnLobbyLeaveReceived(const FString& Message)
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
						FName LeftUsername = FName(payloadObj->GetStringField(TEXT("left_user")));
						if (!LeftUsername.IsEqual(this->GetLobbyInfo().Leader_Username)) {
							FName CurrentLeaderUsername = LobbyInfo.Leader_Username;
							this->RemoveFromLobby(LeftUsername, CurrentLeaderUsername);
						}
						else {
							FString NewLeaderUsername = payloadObj->GetStringField(TEXT("new_leader"));
							this->RemoveFromLobby(LeftUsername, FName(NewLeaderUsername));
						}
					}
				}
			}
		}
	}
}

void UMyGameInstance::OnLobbyInvitationAcceptedReceived(const FString& Message)
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
					FString sender = payloadObj->GetStringField(TEXT("sender"));
					FString receiver = payloadObj->GetStringField(TEXT("receiver"));
					FPlayerInfo NewMember(FName(receiver), true);
					this->AddToLobby(NewMember);
				}
			}
		}
	}
}

void UMyGameInstance::OnMakeLeaderReceived(const FString& Message)
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
						FLobbyInfo NewLobbyInfo(LobbyName, LeaderUsername, LobbyMembers, 5, Status);
						this->SetLobbyInfo(NewLobbyInfo);
					}
				}
			}
		}
	}
}

void UMyGameInstance::OnBeKickFromLobbyReceived(const FString& Message)
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
						FLobbyInfo NewLobbyInfo(LobbyName, LeaderUsername, LobbyMembers, 5, Status);
						this->SetLobbyInfo(NewLobbyInfo);
					}
				}
			}
		}
	}
}
