// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "../../ServiceController/UserAccountController/UserAccountController.h"
#include "../../ServiceController/FriendlistController/FriendlistController.h"
#include "../../ServiceController/LobbyController/LobbyController.h"
#include "../../ServiceController/InGameController/InGameController.h"
#include "../../ServiceController/GameServerController/GameServerController.h"
#include "ServiceControllerSubsystem.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnWSMessageReceive, const FString& /* Status Code */)
UCLASS()
class BEATHIMUP_API UServiceControllerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	FString BASE_URL = "127.0.0.1:3000";

	TSharedPtr<IWebSocket> WS_Connection;

protected:
	

public:
	UServiceControllerSubsystem();

	UPROPERTY()
	TObjectPtr<UUserAccountController> UserAccountController;

	UPROPERTY()
	TObjectPtr<UFriendlistController> FriendlistController;

	UPROPERTY()
	TObjectPtr<ULobbyController> LobbyController;

	UPROPERTY()
	TObjectPtr<UInGameController> InGameController;

	UPROPERTY()
	TObjectPtr<UGameServerController> GameServerController;

	FOnWSMessageReceive WSMessageReceiveDel;

	FString& GetBaseAPIURL() {
		//return FString("http://").Append(BASE_URL);
		return BASE_URL;
	}

	void SetBaseAPIURL(const FString& inURL) {
		BASE_URL = inURL;
	}

	void OpenWSConnection();

	void CloseWSConnection();

	void WSConnectedHandle();

	void WSClosedHandle(int32 StatusCode , const FString& Reason, bool bWasClean);

	void WSConnectionErrHandle(const FString& Error);

	void WSMessageRecvHandle(const FString& Message);

	void SendWSMessage(const FString& Message);
};
