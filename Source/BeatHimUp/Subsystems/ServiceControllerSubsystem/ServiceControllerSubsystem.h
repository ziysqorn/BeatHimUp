// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "../../ServiceController/UserAccountController/UserAccountController.h"
#include "../../ServiceController/FriendlistController/FriendlistController.h"
#include "ServiceControllerSubsystem.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnWSMessageRecieve, const FString& /* Status Code */)
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

	FOnWSMessageRecieve WSMessageRecieveDel;

	FString GetBaseHTTPURL() {
		return FString("http://").Append(BASE_URL);
	}

	UFUNCTION(Client, Reliable)
	void OpenWSConnection();

	UFUNCTION(Client, Reliable)
	void CloseWSConnection();

	void WSConnectedHandle();

	void WSClosedHandle(int32 StatusCode , const FString& Reason, bool bWasClean);

	void WSConnectionErrHandle(const FString& Error);

	void WSMessageRecvHandle(const FString& Message);

	void SendWSMessage(const FString& Message);
};
