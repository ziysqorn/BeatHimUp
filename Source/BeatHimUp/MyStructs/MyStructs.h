// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyStructs.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EMatchStatus : uint8
{
	MATCH_BEGIN UMETA(DisplayName = "Match Begin"),
	MATCH_PAUSED UMETA(DisplayName = "Match Paused"),
	MATCH_END UMETA(DisplayName = "Match Ended"),
	MATCH_WON UMETA(DisplayName = "Match Won"),
	MATCH_LOST UMETA(DisplayName = "Match Lost")
};

USTRUCT()
struct FPlayerInfo {

	GENERATED_BODY()

	UPROPERTY()
	FName Username;

	UPROPERTY()
	bool isOnline;

	FPlayerInfo() : Username(NAME_None), isOnline(false) {}

	FPlayerInfo(FName inUsername, bool status) : Username(inUsername), isOnline(status) {}

	FPlayerInfo& operator=(const FPlayerInfo& Other) {
		if (this == &Other) {
			return *this;
		}

		this->Username = Other.Username;
		this->isOnline = Other.isOnline;

		return *this;
	}
};

USTRUCT()
struct FFriendRequest {

	GENERATED_BODY()

	UPROPERTY()
	FName Sender_Username;

	UPROPERTY()
	FName Receiver_Username;

	FFriendRequest() : Sender_Username(NAME_None), Receiver_Username(NAME_None) {}

	FFriendRequest(FName inSender, FName inReceiver) : Sender_Username(inSender), Receiver_Username(inReceiver) {}
};

USTRUCT()
struct FLobbyInvitation {

	GENERATED_BODY()

	UPROPERTY()
	FName Sender_Username;

	UPROPERTY()
	FName Receiver_Username;

	FLobbyInvitation() : Sender_Username(NAME_None), Receiver_Username(NAME_None) {}

	FLobbyInvitation(FName inSender, FName inReceiver) : Sender_Username(inSender), Receiver_Username(inReceiver) {}
};

USTRUCT()
struct FLobbyInfo {

	GENERATED_BODY()

	UPROPERTY()
	FName LobbyName;

	UPROPERTY()
	FName Leader_Username;

	UPROPERTY()
	TArray<FPlayerInfo> Members;

	UPROPERTY()
	int MaxMember;

	FLobbyInfo() : LobbyName(NAME_None), Leader_Username(NAME_None), Members(TArray<FPlayerInfo>()), MaxMember(5) {}
	FLobbyInfo(FName inLobbyName, FName inLeader, const TArray<FPlayerInfo>& inMembers, int inMaxMember) : 
		LobbyName(inLobbyName),
		Leader_Username(inLeader),
		Members(inMembers),
		MaxMember(inMaxMember) {}

	FLobbyInfo& operator=(const FLobbyInfo& Other) {
		if (this == &Other) {
			return *this;
		}

		this->LobbyName = Other.LobbyName;
		this->Leader_Username = Other.Leader_Username;
		this->Members = Other.Members;
		this->MaxMember = Other.MaxMember;
		return *this;
	}
};
