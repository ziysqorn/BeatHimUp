// Fill out your copyright notice in the Description page of Project Settings.


#include "FriendlistController.h"
#include "../../Subsystems/ServiceControllerSubsystem/ServiceControllerSubsystem.h"
#include "../../CustomGameInstance/MyGameInstance.h"


UFriendlistController::UFriendlistController()
{
}

void UFriendlistController::SendFriendRequest(const FString& receiver, const FHttpRequestCompleteDelegate& callback)
{
	if (UServiceControllerSubsystem* ServiceController = Cast<UServiceControllerSubsystem>(this->GetOuter())) {
		if (UMyGameInstance* GameInstance = Cast<UMyGameInstance>(ServiceController->GetGameInstance())) {
			FString BASE_URL = ServiceController->GetBaseAPIURL();
			FString realURL = BASE_URL.Append("/friend_request/send");
			FHttpModule& HttpModule = FHttpModule::Get();
			TSharedRef<IHttpRequest> httpRequest = HttpModule.CreateRequest();
			httpRequest->SetVerb("POST");
			httpRequest->SetHeader("Content-Type", "application/json");
			httpRequest->SetHeader("Authorization", FString::Format(TEXT("Bearer {0}"), {GameInstance->GetSecretKey()}));
			realURL.Append(TEXT("?"));
			realURL.Append(FString::Format(TEXT("receiver={0}"), { receiver }));
			httpRequest->SetURL(realURL);
			httpRequest->OnProcessRequestComplete() = callback;
			httpRequest->ProcessRequest();
		}
	}
}

void UFriendlistController::GetFriendRequest(const FHttpRequestCompleteDelegate& callback)
{
	if (UServiceControllerSubsystem* ServiceController = Cast<UServiceControllerSubsystem>(this->GetOuter())) {
		if (UMyGameInstance* GameInstance = Cast<UMyGameInstance>(ServiceController->GetGameInstance())) {
			FString BASE_URL = ServiceController->GetBaseAPIURL();
			FString realURL = BASE_URL.Append("/friend_request/get");
			FHttpModule& HttpModule = FHttpModule::Get();
			TSharedRef<IHttpRequest> httpRequest = HttpModule.CreateRequest();
			httpRequest->SetVerb("GET");
			httpRequest->SetHeader("Content-Type", "application/json");
			httpRequest->SetHeader("Authorization", FString::Format(TEXT("Bearer {0}"), { GameInstance->GetSecretKey() }));
			httpRequest->SetURL(realURL);
			httpRequest->OnProcessRequestComplete() = callback;
			httpRequest->ProcessRequest();
		}
	}
}

void UFriendlistController::AcceptFriendRequest(const FString& sender, const FHttpRequestCompleteDelegate& callback)
{
	if (UServiceControllerSubsystem* ServiceController = Cast<UServiceControllerSubsystem>(this->GetOuter())) {
		if (UMyGameInstance* GameInstance = Cast<UMyGameInstance>(ServiceController->GetGameInstance())) {
			FString BASE_URL = ServiceController->GetBaseAPIURL();
			FString realURL = BASE_URL.Append("/friend_request/accept");
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

void UFriendlistController::DeclineFriendRequest(const FString& sender, const FHttpRequestCompleteDelegate& callback)
{
	if (UServiceControllerSubsystem* ServiceController = Cast<UServiceControllerSubsystem>(this->GetOuter())) {
		if (UMyGameInstance* GameInstance = Cast<UMyGameInstance>(ServiceController->GetGameInstance())) {
			FString BASE_URL = ServiceController->GetBaseAPIURL();
			FString realURL = BASE_URL.Append("/friend_request/decline");
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

void UFriendlistController::RemoveFriend(const FString& removedFriend, const FHttpRequestCompleteDelegate& callback)
{
	if (UServiceControllerSubsystem* ServiceController = Cast<UServiceControllerSubsystem>(this->GetOuter())) {
		if (UMyGameInstance* GameInstance = Cast<UMyGameInstance>(ServiceController->GetGameInstance())) {
			FString BASE_URL = ServiceController->GetBaseAPIURL();
			FString realURL = BASE_URL.Append("/friend/remove");
			FHttpModule& HttpModule = FHttpModule::Get();
			TSharedRef<IHttpRequest> httpRequest = HttpModule.CreateRequest();
			httpRequest->SetVerb("POST");
			httpRequest->SetHeader("Content-Type", "application/json");
			httpRequest->SetHeader("Authorization", FString::Format(TEXT("Bearer {0}"), { GameInstance->GetSecretKey() }));
			realURL.Append(TEXT("?"));
			realURL.Append(FString::Format(TEXT("removed_friend={0}"), { removedFriend }));
			httpRequest->SetURL(realURL);
			httpRequest->OnProcessRequestComplete() = callback;
			httpRequest->ProcessRequest();
		}
	}
}

void UFriendlistController::GetFriendlist(const FHttpRequestCompleteDelegate& callback)
{
	if (UServiceControllerSubsystem* ServiceController = Cast<UServiceControllerSubsystem>(this->GetOuter())) {
		if (UMyGameInstance* GameInstance = Cast<UMyGameInstance>(ServiceController->GetGameInstance())) {
			FString BASE_URL = ServiceController->GetBaseAPIURL();
			FString realURL = BASE_URL.Append("/friendlist/get");
			FHttpModule& HttpModule = FHttpModule::Get();
			TSharedRef<IHttpRequest> httpRequest = HttpModule.CreateRequest();
			httpRequest->SetVerb("GET");
			httpRequest->SetHeader("Content-Type", "application/json");
			httpRequest->SetHeader("Authorization", FString::Format(TEXT("Bearer {0}"), { GameInstance->GetSecretKey() }));
			httpRequest->SetURL(realURL);
			httpRequest->OnProcessRequestComplete() = callback;
			httpRequest->ProcessRequest();
		}
	}
}
