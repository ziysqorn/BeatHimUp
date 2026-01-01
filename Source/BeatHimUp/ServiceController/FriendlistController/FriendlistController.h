// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "../../MyStructs/MyStructs.h"
#include "FriendlistController.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API UFriendlistController : public UObject
{
	GENERATED_BODY()

public:
	UFriendlistController();

	void SendFriendRequest(const FString& receiver, const FHttpRequestCompleteDelegate& callback);
	void GetFriendRequest(const FHttpRequestCompleteDelegate& callback);
	void AcceptFriendRequest(const FString& sender, const FHttpRequestCompleteDelegate& callback);
	void DeclineFriendRequest(const FString& sender, const FHttpRequestCompleteDelegate& callback);
	void RemoveFriend(const FString& username1, const FString& username2, const FHttpRequestCompleteDelegate& callback);
	void GetFriendlist(const FHttpRequestCompleteDelegate& callback);
};
