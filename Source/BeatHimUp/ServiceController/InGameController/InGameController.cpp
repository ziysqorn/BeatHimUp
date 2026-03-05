// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameController.h"
#include "../../Subsystems/ServiceControllerSubsystem/ServiceControllerSubsystem.h"
#include "../../CustomGameInstance/MyGameInstance.h"

UInGameController::UInGameController()
{
}

void UInGameController::GetCharacterStats(const FHttpRequestCompleteDelegate& callback)
{
	if (UServiceControllerSubsystem* ServiceController = Cast<UServiceControllerSubsystem>(this->GetOuter())) {
		if (UMyGameInstance* GameInstance = Cast<UMyGameInstance>(ServiceController->GetGameInstance())) {
			FString BASE_URL = ServiceController->GetBaseAPIURL();
			FString realURL = BASE_URL.Append("/in_game/character_stats/get");
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

void UInGameController::SaveCharacterStats(const FString& contentString, const FHttpRequestCompleteDelegate& callback)
{
	if (UServiceControllerSubsystem* ServiceController = Cast<UServiceControllerSubsystem>(this->GetOuter())) {
		if (UMyGameInstance* GameInstance = Cast<UMyGameInstance>(ServiceController->GetGameInstance())) {
			FString BASE_URL = ServiceController->GetBaseAPIURL();
			FString realURL = BASE_URL.Append("/in_game/character_stats/save");
			FHttpModule& HttpModule = FHttpModule::Get();
			TSharedRef<IHttpRequest> httpRequest = HttpModule.CreateRequest();
			httpRequest->SetVerb("POST");
			httpRequest->SetHeader("Content-Type", "application/json");
			httpRequest->SetHeader("Authorization", FString::Format(TEXT("Bearer {0}"), { GameInstance->GetSecretKey() }));
			httpRequest->SetContentAsString(contentString);
			httpRequest->SetURL(realURL);
			httpRequest->OnProcessRequestComplete() = callback;
			httpRequest->ProcessRequest();
		}
	}
}
