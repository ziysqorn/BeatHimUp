// Fill out your copyright notice in the Description page of Project Settings.


#include "ServiceControllerSubsystem.h"
#include "../../CustomGameInstance/MyGameInstance.h"

UServiceControllerSubsystem::UServiceControllerSubsystem()
{
	UserAccountController = NewObject<UUserAccountController>(this, FName("UserAccountController"));
	FriendlistController = NewObject<UFriendlistController>(this, FName("FriendlistController"));
	LobbyController = NewObject<ULobbyController>(this, FName("LobbyController"));
	InGameController = NewObject<UInGameController>(this, FName("InGameController"));
	GameServerController = NewObject<UGameServerController>(this, FName("GameServerController"));
}

void UServiceControllerSubsystem::WSConnectedHandle()
{

}

void UServiceControllerSubsystem::WSClosedHandle(int32 StatusCode, const FString& Reason, bool bWasClean)
{
	if (WS_Connection.IsValid()) {
		WS_Connection->OnConnected().Clear();
		WS_Connection->OnClosed().Clear();
		WS_Connection->OnConnectionError().Clear();
		WS_Connection->OnMessage().Clear();
		WSMessageReceiveDel.Clear();
	}
}

void UServiceControllerSubsystem::WSConnectionErrHandle(const FString& Error)
{
}

void UServiceControllerSubsystem::WSMessageRecvHandle(const FString& Message)
{
	if (WS_Connection.IsValid() && WSMessageReceiveDel.IsBound()) {
		WSMessageReceiveDel.Broadcast(Message);
	}
}

void UServiceControllerSubsystem::SendWSMessage(const FString& Message)
{
	if (WS_Connection.IsValid() && WS_Connection->IsConnected()) {
		WS_Connection->Send(Message);
	}
}


void UServiceControllerSubsystem::CloseWSConnection()
{
	if (WS_Connection.IsValid() && WS_Connection->IsConnected())
		WS_Connection->Close();
}

void UServiceControllerSubsystem::OpenWSConnection()
{
	if (UMyGameInstance* GameInstance = Cast<UMyGameInstance>(this->GetGameInstance())) {
		FString webSocketURL = FString("ws://").Append(BASE_URL).Append("/ws");
		FString realURL = webSocketURL.Append("?").Append(FString::Format(TEXT("username={0}"), { GameInstance->GetPlayerInfo().Username.ToString() }));
		WS_Connection = FWebSocketsModule::Get().CreateWebSocket(realURL);
	}
	if (WS_Connection.IsValid() && !WS_Connection->IsConnected()) {
		WS_Connection->OnConnected().AddUObject(this, &UServiceControllerSubsystem::WSConnectedHandle);
		WS_Connection->OnClosed().AddUObject(this, &UServiceControllerSubsystem::WSClosedHandle);
		WS_Connection->OnConnectionError().AddUObject(this, &UServiceControllerSubsystem::WSConnectionErrHandle);
		WS_Connection->OnMessage().AddUObject(this, &UServiceControllerSubsystem::WSMessageRecvHandle);
		WS_Connection->Connect();
	}
}
