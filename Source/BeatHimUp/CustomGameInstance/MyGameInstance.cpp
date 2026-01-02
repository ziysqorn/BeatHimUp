// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "../Subsystems/ServiceControllerSubsystem/ServiceControllerSubsystem.h"

void UMyGameInstance::Init()
{
	Super::Init();

	FCoreDelegates::OnHandleSystemError.AddUObject(this, &UMyGameInstance::SystemShutdownLogout);
}

void UMyGameInstance::Shutdown()
{
	Super::Shutdown();
	SystemShutdownLogout();
}

void UMyGameInstance::SystemShutdownLogout()
{
	if (UServiceControllerSubsystem* ServiceController = this->GetSubsystem<UServiceControllerSubsystem>()) {
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
			this->SecretToken = TEXT("");
			this->PlayerInfo.Username = NAME_None;
			this->PlayerInfo.isOnline = false;
			this->Friendlist.Empty();
			if (UServiceControllerSubsystem* ServiceController = this->GetSubsystem<UServiceControllerSubsystem>()) {
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
}

int UMyGameInstance::RemoveFromFriendlist(FName target)
{
	for (int i = 0; i < Friendlist.Num(); ++i) {
		if (Friendlist[i].Username.IsEqual(target)) {
			if (Friendlist[i].isOnline)
				CurrentOnlineFriendNum -= 1;
			Friendlist.RemoveAt(i, EAllowShrinking::No);
			return i;
		}
	}
	return -1;
}

void UMyGameInstance::RefreshFriendRequest(const TArray<TSharedPtr<FJsonValue>>& jsonObjArr)
{
	FriendRequestList.Empty();
	for (int i = 0; i < jsonObjArr.Num(); ++i) {
		TSharedPtr<FJsonObject> friendRequestObj = jsonObjArr[i]->AsObject();
		FString sender = friendRequestObj->GetStringField(TEXT("sender"));
		FString receiver = friendRequestObj->GetStringField(TEXT("receiver"));
		FFriendRequest NewFriendRequest;
		NewFriendRequest.Sender_Username = FName(sender);
		NewFriendRequest.Receiver_Username = FName(receiver);
		FriendRequestList.Add(NewFriendRequest);
	}
}

void UMyGameInstance::RefreshFriendlist(const TArray<TSharedPtr<FJsonValue>>& jsonObjArr)
{
	Friendlist.Empty();
	CurrentOnlineFriendNum = 0;
	for (const TSharedPtr<FJsonValue>& Value : jsonObjArr) {
		TSharedPtr<FJsonObject> friendJsonObj = Value->AsObject();
		if (friendJsonObj.IsValid()) {
			FPlayerInfo newPlayerInfo;
			newPlayerInfo.Username = FName(friendJsonObj->GetStringField(TEXT("username")));
			newPlayerInfo.isOnline = friendJsonObj->GetBoolField(TEXT("status"));
			if (newPlayerInfo.isOnline) ++CurrentOnlineFriendNum;
			Friendlist.Add(newPlayerInfo);
		}
	}
	Friendlist.Sort([](const FPlayerInfo& player1, const FPlayerInfo& player2) {
		return player1.isOnline && !player2.isOnline;
		});
}

int UMyGameInstance::RemoveFromLobbyInvitationList(FName target)
{
	for (int i = 0; i < LobbyInvitationList.Num(); ++i) {
		if (LobbyInvitationList[i].Sender_Username.IsEqual(target)) {
			LobbyInvitationList.RemoveAt(i, EAllowShrinking::No);
			return i;
		}
	}
	return -1;
}
