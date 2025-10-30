// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "../Subsystems/ServiceControllerSubsystem/ServiceControllerSubsystem.h"

void UMyGameInstance::Init()
{
	Super::Init();

	FCoreDelegates::OnHandleSystemError.AddUObject(this, &UMyGameInstance::SystemShutdownLogout);
}

void UMyGameInstance::Shutdown()
{
	Super::Shutdown();
	SystemShutdownLogout();
}

void UMyGameInstance::SystemShutdownLogout()
{
	if (UServiceControllerSubsystem* ServiceController = this->GetSubsystem<UServiceControllerSubsystem>()) {
		if (IsValid(ServiceController->UserAccountController)) {
			FHttpRequestCompleteDelegate LogoutRequestCompleteDel;
			LogoutRequestCompleteDel.BindUObject(this, &UMyGameInstance::LogoutRequestComplete);
			ServiceController->UserAccountController->LogoutUser(this->PlayerInfo.Username.ToString(), LogoutRequestCompleteDel);
		}
	}
}

void UMyGameInstance::LogoutRequestComplete(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully) {
	check(IsInGameThread());
	if (connectedSuccessfully && pResponse.IsValid()) {
		switch (pResponse->GetResponseCode()) {
		case EHttpResponseCodes::Ok:
			this->PlayerInfo.Username = NAME_None;
			this->PlayerInfo.isOnline = false;
			this->Friendlist.Empty();
			break;
		}
	}
}
