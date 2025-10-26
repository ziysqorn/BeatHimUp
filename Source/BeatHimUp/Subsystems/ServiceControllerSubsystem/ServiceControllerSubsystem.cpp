// Fill out your copyright notice in the Description page of Project Settings.


#include "ServiceControllerSubsystem.h"

UServiceControllerSubsystem::UServiceControllerSubsystem()
{
	UserAccountController = NewObject<UUserAccountController>(this, FName("UserAccountController"));
	FriendlistController = NewObject<UFriendlistController>(this, FName("FriendlistController"));
	if (!WS_Connection.IsValid()) {
		FString realURL = FString("ws://").Append(BASE_URL).Append("/ws");
		WS_Connection = FWebSocketsModule::Get().CreateWebSocket(realURL);
	}
}

void UServiceControllerSubsystem::WSConnectedHandle()
{

}

void UServiceControllerSubsystem::WSClosedHandle(int32 StatusCode, const FString& Reason, bool bWasClean)
{

}

void UServiceControllerSubsystem::WSConnectionErrHandle(const FString& Error)
{
}

void UServiceControllerSubsystem::WSMessageRecvHandle(const FString& Message)
{
	if (WS_Connection.IsValid() && WSMessageRecieveDel.IsBound()) {
		WSMessageRecieveDel.Broadcast(Message);
	}
}

void UServiceControllerSubsystem::SendWSMessage(const FString& Message)
{
	if (WS_Connection.IsValid() && WS_Connection->IsConnected()) {
		WS_Connection->Send(Message);
	}
}


void UServiceControllerSubsystem::CloseWSConnection_Implementation()
{
	if (WS_Connection.IsValid() && WS_Connection->IsConnected())
		WS_Connection->Close();
}

void UServiceControllerSubsystem::OpenWSConnection_Implementation()
{
	if (WS_Connection.IsValid() && !WS_Connection->IsConnected()) {
		WS_Connection->OnConnected().AddUObject(this, &UServiceControllerSubsystem::WSConnectedHandle);
		WS_Connection->OnClosed().AddUObject(this, &UServiceControllerSubsystem::WSClosedHandle);
		WS_Connection->OnConnectionError().AddUObject(this, &UServiceControllerSubsystem::WSConnectionErrHandle);
		WS_Connection->OnMessage().AddUObject(this, &UServiceControllerSubsystem::WSMessageRecvHandle);
		WS_Connection->Connect();
	}
}
