// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "UserAccountController.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API UUserAccountController : public UObject
{
	GENERATED_BODY()
	
public:
	void CreateUser(const FString& contentString, const FHttpRequestCompleteDelegate& callback);
	void LoginUser(const FString& contentString, const FHttpRequestCompleteDelegate& callback);
	void LogoutUser(const FString& username, const FHttpRequestCompleteDelegate& callback);
	void AddFriend(const FString& username1, const FString& username2, const FHttpRequestCompleteDelegate& callback);
	void RemoveFriend(const FString& username1, const FString& username2, const FHttpRequestCompleteDelegate& callback);
	void GetFriendlist(const FString& username, const FHttpRequestCompleteDelegate& callback);
};
