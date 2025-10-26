// Fill out your copyright notice in the Description page of Project Settings.


#include "UserAccountController.h"
#include "../../Subsystems/ServiceControllerSubsystem/ServiceControllerSubsystem.h"

void UUserAccountController::CreateUser(const FString& contentString, const FHttpRequestCompleteDelegate& callback)
{
	if (UServiceControllerSubsystem* ServiceController = Cast<UServiceControllerSubsystem>(this->GetOuter())) {
		FString BASE_URL = ServiceController->GetBaseHTTPURL();
		FString realURL = BASE_URL.Append("/user/create");
		FHttpModule& HttpModule = FHttpModule::Get();
		TSharedRef<IHttpRequest> httpRequest = HttpModule.CreateRequest();
		httpRequest->SetVerb("POST");
		httpRequest->SetHeader("Content-Type", "application/json");
		httpRequest->SetURL(realURL);
		httpRequest->SetContentAsString(contentString);
		httpRequest->OnProcessRequestComplete() = callback;
		httpRequest->ProcessRequest();
	}
}

void UUserAccountController::LoginUser(const FString& contentString, const FHttpRequestCompleteDelegate& callback)
{
	if (UServiceControllerSubsystem* ServiceController = Cast<UServiceControllerSubsystem>(this->GetOuter())) {
		FString BASE_URL = ServiceController->GetBaseHTTPURL();
		FString realURL = BASE_URL.Append("/user/login");
		FHttpModule& HttpModule = FHttpModule::Get();
		TSharedRef<IHttpRequest> httpRequest = HttpModule.CreateRequest();
		httpRequest->SetVerb("POST");
		httpRequest->SetHeader("Content-Type", "application/json");
		httpRequest->SetURL(realURL);
		httpRequest->SetContentAsString(contentString);
		httpRequest->OnProcessRequestComplete() = callback;
		httpRequest->ProcessRequest();
	}
}

void UUserAccountController::LogoutUser(const FString& username, const FHttpRequestCompleteDelegate& callback)
{
	if (UServiceControllerSubsystem* ServiceController = Cast<UServiceControllerSubsystem>(this->GetOuter())) {
		FString BASE_URL = ServiceController->GetBaseHTTPURL();
		FString realURL = BASE_URL.Append("/user/logout");
		FHttpModule& HttpModule = FHttpModule::Get();
		TSharedRef<IHttpRequest> httpRequest = HttpModule.CreateRequest();
		httpRequest->SetVerb("POST");
		httpRequest->SetHeader("Content-Type", "application/json");
		realURL.Append(TEXT("?"));
		realURL.Append(FString::Format(TEXT("username={0}"), { username }));
		httpRequest->SetURL(realURL);
		httpRequest->OnProcessRequestComplete() = callback;
		httpRequest->ProcessRequest();
	}
}

void UUserAccountController::AddFriend(const FString& username1, const FString& username2, const FHttpRequestCompleteDelegate& callback)
{
	if (UServiceControllerSubsystem* ServiceController = Cast<UServiceControllerSubsystem>(this->GetOuter())) {
		FString BASE_URL = ServiceController->GetBaseHTTPURL();
		FString realURL = BASE_URL.Append("/user/friendlist/add");
		FHttpModule& HttpModule = FHttpModule::Get();
		TSharedRef<IHttpRequest> httpRequest = HttpModule.CreateRequest();
		httpRequest->SetVerb("POST");
		httpRequest->SetHeader("Content-Type", "application/json");
		realURL.Append(TEXT("?"));
		realURL.Append(FString::Format(TEXT("player1={0}"), { username1 }));
		realURL.Append(TEXT("&"));
		realURL.Append(FString::Format(TEXT("player2={0}"), { username2 }));
		httpRequest->SetURL(realURL);
		httpRequest->OnProcessRequestComplete() = callback;
		httpRequest->ProcessRequest();
	}
}

void UUserAccountController::RemoveFriend(const FString& username1, const FString& username2, const FHttpRequestCompleteDelegate& callback)
{
	if (UServiceControllerSubsystem* ServiceController = Cast<UServiceControllerSubsystem>(this->GetOuter())) {
		FString BASE_URL = ServiceController->GetBaseHTTPURL();
		FString realURL = BASE_URL.Append("/user/friendlist/remove");
		FHttpModule& HttpModule = FHttpModule::Get();
		TSharedRef<IHttpRequest> httpRequest = HttpModule.CreateRequest();
		httpRequest->SetVerb("POST");
		httpRequest->SetHeader("Content-Type", "application/json");
		realURL.Append(TEXT("?"));
		realURL.Append(FString::Format(TEXT("player1={0}"), { username1 }));
		realURL.Append(TEXT("&"));
		realURL.Append(FString::Format(TEXT("player2={0}"), { username2 }));
		httpRequest->SetURL(realURL);
		httpRequest->OnProcessRequestComplete() = callback;
		httpRequest->ProcessRequest();
	}
}

void UUserAccountController::GetFriendlist(const FString& username, const FHttpRequestCompleteDelegate& callback)
{
	if (UServiceControllerSubsystem* ServiceController = Cast<UServiceControllerSubsystem>(this->GetOuter())) {
		FString BASE_URL = ServiceController->GetBaseHTTPURL();
		FString realURL = BASE_URL.Append("/user/friendlist/get");
		FHttpModule& HttpModule = FHttpModule::Get();
		TSharedRef<IHttpRequest> httpRequest = HttpModule.CreateRequest();
		httpRequest->SetVerb("GET");
		httpRequest->SetHeader("Content-Type", "application/json");
		realURL.Append(TEXT("?"));
		realURL.Append(FString::Format(TEXT("username={0}"), { username }));
		httpRequest->SetURL(realURL);
		httpRequest->OnProcessRequestComplete() = callback;
		httpRequest->ProcessRequest();
	}
}
