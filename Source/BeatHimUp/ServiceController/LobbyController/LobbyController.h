// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "../../MyStructs/MyStructs.h"
#include "LobbyController.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API ULobbyController : public UObject
{
	GENERATED_BODY()

public:
	ULobbyController();

	void InviteToLobby(const FString& receiver, const FHttpRequestCompleteDelegate& callback);
	void AcceptLobbyInvitation(const FString& sender, const FHttpRequestCompleteDelegate& callback);
	void DeclineLobbyInvitation(const FString& sender, const FHttpRequestCompleteDelegate& callback);
};
