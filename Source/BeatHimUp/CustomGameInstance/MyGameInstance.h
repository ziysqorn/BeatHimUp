// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ProjectIncludes.h"
#include "../MyStructs/MyStructs.h"
#include "../DataAsset/UIDataAsset.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnLobbyUpdate, const FLobbyInfo&)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnLobbyIDChange, const FString&)

UCLASS()
class BEATHIMUP_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	FString SecretToken = TEXT("");

	UPROPERTY()
	FPlayerInfo PlayerInfo;

	UPROPERTY()
	FLobbyInfo LobbyInfo;

	UPROPERTY()
	TArray<FPlayerInfo> Friendlist;

	UPROPERTY()
	TMap<FName, int> FriendlistIdxMap;

	UPROPERTY()
	TArray<FFriendRequest> FriendRequestList;

	UPROPERTY()
	TArray<FLobbyInvitation> LobbyInvitationList;

	UPROPERTY()
	TMap<FName, int> LobbyInvitationIdxMap;

	int CurrentOnlineFriendNum = 0;

	bool bMustShutdown = false;

	UPROPERTY(EditDefaultsOnly, Category = "DA_UI")
	TObjectPtr<UUIDataAsset> DA_UI;

	void Init() override;
	void Shutdown() override;

	void ApplySavedGraphicSettings();

	void OnGetDotgIPComplete(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully);

	void GetDotgIP(const FHttpRequestCompleteDelegate& callback);

public:
	FOnLobbyUpdate OnLobbyUpdateDel;

	FOnLobbyIDChange OnLobbyIDChangeDel;

	void LogoutProcess();

	void CrashHandle();

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

	int CheckIsFriend(FName Target) {
		int* Found = FriendlistIdxMap.Find(Target);
		return Found ? *FriendlistIdxMap.Find(Target) : -1;
	}

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

	void InsertLobbyInvitation(const FLobbyInvitation& inInvitation, int idx);

	int RemoveFromLobbyInvitationList(FName target);

	const TArray<FLobbyInvitation>& GetLobbyInvitationList() {
		return LobbyInvitationList;
	}

	const FLobbyInfo& GetLobbyInfo() {
		return LobbyInfo;
	}

	void SetLobbyInfo(const FLobbyInfo& inLobby) {
		LobbyInfo = inLobby;
		if (OnLobbyUpdateDel.IsBound()) {
			OnLobbyUpdateDel.Broadcast(LobbyInfo);
		}
	}

	void SetLobbyLeader(FName NewLeader) {
		LobbyInfo.Leader_Username = NewLeader;
		if (OnLobbyUpdateDel.IsBound()) {
			OnLobbyUpdateDel.Broadcast(LobbyInfo);
		}
	}

	void AddToLobby(const FPlayerInfo& NewMember);

	int RemoveFromLobby(FName UsernameToRemove, FName LeaderUsername);

	void ClearClientInfo();

	void OnPlayerJoinLobby(const FString& Message);

	void OnLobbyInvitationAcceptedReceived(const FString& Message);

	void OnLobbyLeaveReceived(const FString& Message);

	void OnMakeLeaderReceived(const FString& Message);

	void OnBeKickFromLobbyReceived(const FString& Message);
};
