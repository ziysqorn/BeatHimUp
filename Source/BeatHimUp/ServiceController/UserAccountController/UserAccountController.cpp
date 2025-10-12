// Fill out your copyright notice in the Description page of Project Settings.


#include "UserAccountController.h"
#include "../../Subsystems/ServiceControllerSubsystem/ServiceControllerSubsystem.h"

void UUserAccountController::CreateUser(const FString& contentString, const FHttpRequestCompleteDelegate& callback)
{
	if (UServiceControllerSubsystem* ServiceController = Cast<UServiceControllerSubsystem>(this->GetOuter())) {
		FString BASE_URL = ServiceController->GetBaseURL();
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
		FString BASE_URL = ServiceController->GetBaseURL();
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
