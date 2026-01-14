// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyController.h"
#include "../../Subsystems/ServiceControllerSubsystem/ServiceControllerSubsystem.h"
#include "../../CustomGameInstance/MyGameInstance.h"

ULobbyController::ULobbyController()
{
}

void ULobbyController::CreateLobby(const FHttpRequestCompleteDelegate& callback)
{
	if (UServiceControllerSubsystem* ServiceController = Cast<UServiceControllerSubsystem>(this->GetOuter())) {
		if (UMyGameInstance* GameInstance = Cast<UMyGameInstance>(ServiceController->GetGameInstance())) {
			FString BASE_URL = ServiceController->GetBaseHTTPURL();
			FString realURL = BASE_URL.Append("/lobby/create");
			FHttpModule& HttpModule = FHttpModule::Get();
			TSharedRef<IHttpRequest> httpRequest = HttpModule.CreateRequest();
			httpRequest->SetVerb("POST");
			httpRequest->SetHeader("Content-Type", "application/json");
			httpRequest->SetHeader("Authorization", FString::Format(TEXT("Bearer {0}"), { GameInstance->GetSecretKey() }));
			httpRequest->SetURL(realURL);
			httpRequest->OnProcessRequestComplete() = callback;
			httpRequest->ProcessRequest();
		}
	}
}

void ULobbyController::InviteToLobby(const FString& receiver, const FHttpRequestCompleteDelegate& callback)
{
	if (UServiceControllerSubsystem* ServiceController = Cast<UServiceControllerSubsystem>(this->GetOuter())) {
		if (UMyGameInstance* GameInstance = Cast<UMyGameInstance>(ServiceController->GetGameInstance())) {
			FString BASE_URL = ServiceController->GetBaseHTTPURL();
			FString realURL = BASE_URL.Append("/lobby/invite");
			FHttpModule& HttpModule = FHttpModule::Get();
			TSharedRef<IHttpRequest> httpRequest = HttpModule.CreateRequest();
			httpRequest->SetVerb("POST");
			httpRequest->SetHeader("Content-Type", "application/json");
			httpRequest->SetHeader("Authorization", FString::Format(TEXT("Bearer {0}"), { GameInstance->GetSecretKey() }));
			realURL.Append(TEXT("?"));
			realURL.Append(FString::Format(TEXT("receiver={0}"), { receiver }));
			httpRequest->SetURL(realURL);
			httpRequest->OnProcessRequestComplete() = callback;
			httpRequest->ProcessRequest();
		}
	}
}

void ULobbyController::AcceptLobbyInvitation(const FString& sender, const FHttpRequestCompleteDelegate& callback)
{
	if (UServiceControllerSubsystem* ServiceController = Cast<UServiceControllerSubsystem>(this->GetOuter())) {
		if (UMyGameInstance* GameInstance = Cast<UMyGameInstance>(ServiceController->GetGameInstance())) {
			FString BASE_URL = ServiceController->GetBaseHTTPURL();
			FString realURL = BASE_URL.Append("/lobby/accept");
			FHttpModule& HttpModule = FHttpModule::Get();
			TSharedRef<IHttpRequest> httpRequest = HttpModule.CreateRequest();
			httpRequest->SetVerb("POST");
			httpRequest->SetHeader("Content-Type", "application/json");
			httpRequest->SetHeader("Authorization", FString::Format(TEXT("Bearer {0}"), { GameInstance->GetSecretKey() }));
			realURL.Append(TEXT("?"));
			realURL.Append(FString::Format(TEXT("sender={0}"), { sender }));
			httpRequest->SetURL(realURL);
			httpRequest->OnProcessRequestComplete() = callback;
			httpRequest->ProcessRequest();
		}
	}
}

void ULobbyController::LeaveLobby(const FHttpRequestCompleteDelegate& callback) 
{
	if (UServiceControllerSubsystem* ServiceController = Cast<UServiceControllerSubsystem>(this->GetOuter())) {
		if (UMyGameInstance* GameInstance = Cast<UMyGameInstance>(ServiceController->GetGameInstance())) {
			FString BASE_URL = ServiceController->GetBaseHTTPURL();
			FString realURL = BASE_URL.Append("/lobby/leave");
			FHttpModule& HttpModule = FHttpModule::Get();
			TSharedRef<IHttpRequest> httpRequest = HttpModule.CreateRequest();
			httpRequest->SetVerb("POST");
			httpRequest->SetHeader("Content-Type", "application/json");
			httpRequest->SetHeader("Authorization", FString::Format(TEXT("Bearer {0}"), { GameInstance->GetSecretKey() }));
			httpRequest->SetURL(realURL);
			httpRequest->OnProcessRequestComplete() = callback;
			httpRequest->ProcessRequest();
		}
	}
}

void ULobbyController::DeclineLobbyInvitation(const FString& sender, const FHttpRequestCompleteDelegate& callback)
{
	if (UServiceControllerSubsystem* ServiceController = Cast<UServiceControllerSubsystem>(this->GetOuter())) {
		if (UMyGameInstance* GameInstance = Cast<UMyGameInstance>(ServiceController->GetGameInstance())) {
			FString BASE_URL = ServiceController->GetBaseHTTPURL();
			FString realURL = BASE_URL.Append("/lobby/decline");
			FHttpModule& HttpModule = FHttpModule::Get();
			TSharedRef<IHttpRequest> httpRequest = HttpModule.CreateRequest();
			httpRequest->SetVerb("POST");
			httpRequest->SetHeader("Content-Type", "application/json");
			httpRequest->SetHeader("Authorization", FString::Format(TEXT("Bearer {0}"), { GameInstance->GetSecretKey() }));
			realURL.Append(TEXT("?"));
			realURL.Append(FString::Format(TEXT("sender={0}"), { sender }));
			httpRequest->SetURL(realURL);
			httpRequest->OnProcessRequestComplete() = callback;
			httpRequest->ProcessRequest();
		}
	}
}

void ULobbyController::MakeLeader(const FString& receiver, const FHttpRequestCompleteDelegate& callback)
{
	if (UServiceControllerSubsystem* ServiceController = Cast<UServiceControllerSubsystem>(this->GetOuter())) {
		if (UMyGameInstance* GameInstance = Cast<UMyGameInstance>(ServiceController->GetGameInstance())) {
			FString BASE_URL = ServiceController->GetBaseHTTPURL();
			FString realURL = BASE_URL.Append("/lobby/make_leader");
			FHttpModule& HttpModule = FHttpModule::Get();
			TSharedRef<IHttpRequest> httpRequest = HttpModule.CreateRequest();
			httpRequest->SetVerb("POST");
			httpRequest->SetHeader("Content-Type", "application/json");
			httpRequest->SetHeader("Authorization", FString::Format(TEXT("Bearer {0}"), { GameInstance->GetSecretKey() }));
			realURL.Append(TEXT("?"));
			realURL.Append(FString::Format(TEXT("receiver={0}"), { receiver }));
			httpRequest->SetURL(realURL);
			httpRequest->OnProcessRequestComplete() = callback;
			httpRequest->ProcessRequest();
		}
	}
}

void ULobbyController::KickMemberFromLobby(const FString& receiver, const FHttpRequestCompleteDelegate& callback)
{
	if (UServiceControllerSubsystem* ServiceController = Cast<UServiceControllerSubsystem>(this->GetOuter())) {
		if (UMyGameInstance* GameInstance = Cast<UMyGameInstance>(ServiceController->GetGameInstance())) {
			FString BASE_URL = ServiceController->GetBaseHTTPURL();
			FString realURL = BASE_URL.Append("/lobby/kick");
			FHttpModule& HttpModule = FHttpModule::Get();
			TSharedRef<IHttpRequest> httpRequest = HttpModule.CreateRequest();
			httpRequest->SetVerb("POST");
			httpRequest->SetHeader("Content-Type", "application/json");
			httpRequest->SetHeader("Authorization", FString::Format(TEXT("Bearer {0}"), { GameInstance->GetSecretKey() }));
			realURL.Append(TEXT("?"));
			realURL.Append(FString::Format(TEXT("receiver={0}"), { receiver }));
			httpRequest->SetURL(realURL);
			httpRequest->OnProcessRequestComplete() = callback;
			httpRequest->ProcessRequest();
		}
	}
}

void ULobbyController::StartGame(const FHttpRequestCompleteDelegate& callback)
{
	if (UServiceControllerSubsystem* ServiceController = Cast<UServiceControllerSubsystem>(this->GetOuter())) {
		if (UMyGameInstance* GameInstance = Cast<UMyGameInstance>(ServiceController->GetGameInstance())) {
			FString BASE_URL = ServiceController->GetBaseHTTPURL();
			FString realURL = BASE_URL.Append("/game_server/create");
			FHttpModule& HttpModule = FHttpModule::Get();
			TSharedRef<IHttpRequest> httpRequest = HttpModule.CreateRequest();
			httpRequest->SetVerb("POST");
			httpRequest->SetHeader("Content-Type", "application/json");
			httpRequest->SetHeader("Authorization", FString::Format(TEXT("Bearer {0}"), { GameInstance->GetSecretKey() }));
			httpRequest->SetURL(realURL);
			httpRequest->OnProcessRequestComplete() = callback;
			httpRequest->ProcessRequest();
		}
	}
}
