// Fill out your copyright notice in the Description page of Project Settings.


#include "GameServerController.h"
#include "../../Subsystems/ServiceControllerSubsystem/ServiceControllerSubsystem.h"
#include "../../CustomGameInstance/MyGameInstance.h"

UGameServerController::UGameServerController()
{
}

void UGameServerController::DropGameServer(const FString& Server_ID, const FHttpRequestCompleteDelegate& callback)
{
	if (UServiceControllerSubsystem* ServiceController = Cast<UServiceControllerSubsystem>(this->GetOuter())) {
		FString BASE_URL = ServiceController->GetBaseAPIURL();
		FString realURL = BASE_URL.Append("/game_server/drop");
		FHttpModule& HttpModule = FHttpModule::Get();
		TSharedRef<IHttpRequest> httpRequest = HttpModule.CreateRequest();
		httpRequest->SetVerb("POST");
		httpRequest->SetHeader("Content-Type", "application/json");
		realURL.Append(TEXT("?"));
		realURL.Append(FString::Format(TEXT("server_id={0}"), { Server_ID }));
		httpRequest->SetURL(realURL);
		httpRequest->OnProcessRequestComplete() = callback;
		httpRequest->ProcessRequest();
	}
}
