// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerState.h"
#include "../CustomGameInstance/MyGameInstance.h"
#include "../GameMode/MainGameMode/MainGameMode.h"


AMainPlayerState::AMainPlayerState()
{
}

void AMainPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (GetLocalRole() == ENetRole::ROLE_AutonomousProxy) {
		if (UMyGameInstance* MyGameInstance = GetGameInstance<UMyGameInstance>()) {
			MyGameInstance->OnLobbyUpdateDel.AddUObject(this, &AMainPlayerState::Server_SendLobbyInfoToPlayerState);
			MyGameInstance->OnLobbyIDChangeDel.AddUObject(this, &AMainPlayerState::Server_SendLobbyID);
		}
	}
}

void AMainPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMainPlayerState, PlayerInfo);

	DOREPLIFETIME(AMainPlayerState, LobbyInfo);
}

void AMainPlayerState::OnRep_PlayerInfo()
{
	if (OnPlayerInfoRepDel.IsBound()) {
		OnPlayerInfoRepDel.Broadcast(PlayerInfo);
	}
}

void AMainPlayerState::OnRep_LobbyInfo()
{
}

void AMainPlayerState::Server_SendPlayerInfoToPlayerState_Implementation(const FPlayerInfo& Info)
{
	PlayerInfo = Info;
}

void AMainPlayerState::Server_SendLobbyInfoToPlayerState_Implementation(const FLobbyInfo& Info)
{
	LobbyInfo = Info;
}

void AMainPlayerState::Server_SendLobbyID_Implementation(const FString& LobbyID)
{
	if (LobbyInfo.Leader_Username.IsEqual(PlayerInfo.Username)) {
		if (AMainGameMode* MainGM = GetWorld()->GetAuthGameMode<AMainGameMode>()) {
			MainGM->SetServerID(LobbyID);
		}
	}
}

void AMainPlayerState::Client_RequestPlayerInfo_Implementation()
{
	if (UMyGameInstance* MyGameInstance = GetGameInstance<UMyGameInstance>()) {
		Server_SendPlayerInfoToPlayerState(MyGameInstance->GetPlayerInfo());
	}
}

void AMainPlayerState::Client_RequestLobbyInfo_Implementation()
{
	if (UMyGameInstance* MyGameInstance = GetGameInstance<UMyGameInstance>()) {
		Server_SendLobbyInfoToPlayerState(MyGameInstance->GetLobbyInfo());
	}
}

 