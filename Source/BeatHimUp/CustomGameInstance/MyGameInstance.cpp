// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "../Subsystems/ServiceControllerSubsystem/ServiceControllerSubsystem.h"
#include "../Subsystems/UIManager/UIManagerSubsystem.h"

void UMyGameInstance::Init()
{
	Super::Init();

	FCoreDelegates::OnHandleSystemError.AddUObject(this, &UMyGameInstance::LogoutProcess);
	if (UUIManagerSubsystem* UISubs = GetSubsystem<UUIManagerSubsystem>()) {
		FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(UISubs, &UUIManagerSubsystem::PostLoadMapPreparation);
	}
}

void UMyGameInstance::Shutdown()
{
	Super::Shutdown();
	LogoutProcess();
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
		}
	}
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

void UMyGameInstance::SetupGraphicsPresets(int Quality)
{
	if (GEngine) {
		if (UGameUserSettings* GameUserSettings = GEngine->GetGameUserSettings()) {
			GameUserSettings->SetViewDistanceQuality(Quality);
			GameUserSettings->SetAntiAliasingQuality(Quality);
			GameUserSettings->SetShadowQuality(Quality);
			GameUserSettings->SetPostProcessingQuality(Quality);
			GameUserSettings->SetTextureQuality(Quality);
			GameUserSettings->SetVisualEffectQuality(Quality);
		}
	}
}
