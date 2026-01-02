// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ProjectIncludes.h"
#include "../MyStructs/MyStructs.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	FString SecretToken = TEXT("");

	UPROPERTY()
	FPlayerInfo PlayerInfo;

	TArray<FPlayerInfo> Friendlist;

	TArray<FFriendRequest> FriendRequestList;

	TArray<FLobbyInvitation> LobbyInvitationList;

	int CurrentOnlineFriendNum = 0;

	void Init() override;
	void Shutdown() override;
	void SystemShutdownLogout();
public:
	void LogoutRequestComplete(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully);

	const FString& GetSecretKey() {
		return SecretToken;
	}

	void SetSecretToken(const FString& inToken) {
		if (SecretToken.IsEmpty()) {
			SecretToken = inToken;
		}
	}

	const FPlayerInfo& GetPlayerInfo() {
		return PlayerInfo;
	}

	int GetCurrentOnlineFriendNum() {
		return CurrentOnlineFriendNum;
	}

	int GetTotalFriendNum() {
		return Friendlist.Num();
	}

	void SetPlayerInfo(const FPlayerInfo& inPlayer) {
		PlayerInfo.Username = inPlayer.Username;
		PlayerInfo.isOnline = inPlayer.isOnline;
	}

	const TArray<FPlayerInfo>& GetFriendlist() {
		return Friendlist;
	}

	void AddToFriendlist(const FPlayerInfo& inPlayer);

	int RemoveFromFriendlist(FName target);

	const TArray<FFriendRequest>& GetFriendRequestList() {
		return FriendRequestList;
	}

	int GetFriendRequestSenderIdx(FName inSender) 
	{
		for (int i = 0; i < FriendRequestList.Num(); ++i) {
			if (FriendRequestList[i].Sender_Username.IsEqual(inSender)) {
				return i;
			}
		}
		return -1;
	}

	int RemoveFriendRequestBySender(FName inSender) {
		for (int i = 0; i < FriendRequestList.Num(); ++i) {
			if (FriendRequestList[i].Sender_Username.IsEqual(inSender)) {
				FriendRequestList.RemoveAt(i, EAllowShrinking::No);
				return i;
			}
		}
		return -1;
	}

	void InsertFriendRequest(const FFriendRequest& inFriendRequest, int idx) {
		FriendRequestList.Insert(inFriendRequest, idx);
	}

	void RefreshFriendRequest(const TArray<TSharedPtr<FJsonValue>>& jsonObjArr);

	void RefreshFriendlist(const TArray<TSharedPtr<FJsonValue>>& jsonObjArr);

	void InsertLobbyInvitation(const FLobbyInvitation& inInvitation, int idx) {
		LobbyInvitationList.Insert(inInvitation, idx);
	}

	int RemoveFromLobbyInvitationList(FName target);

	const TArray<FLobbyInvitation>& GetLobbyInvitationList() {
		return LobbyInvitationList;
	}
};
