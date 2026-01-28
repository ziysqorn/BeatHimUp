// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ProjectIncludes.h"
#include "MainPlayerState.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerInfoRep, const FPlayerInfo&)

UCLASS()
class BEATHIMUP_API AMainPlayerState : public APlayerState
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_PlayerInfo)
	FPlayerInfo PlayerInfo;

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_LobbyInfo)
	FLobbyInfo LobbyInfo;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_PlayerInfo();

	UFUNCTION()
	void OnRep_LobbyInfo();

	UFUNCTION(Server, Reliable)
	void Server_SendPlayerInfoToPlayerState(const FPlayerInfo& Info);

	UFUNCTION(Server, Reliable)
	void Server_SendLobbyInfoToPlayerState(const FLobbyInfo& Info);

	UFUNCTION(Server, Reliable)
	void Server_SendLobbyID(const FString& LobbyID);

	void BeginPlay() override;

public:
	AMainPlayerState();

	FOnPlayerInfoRep OnPlayerInfoRepDel;

	UFUNCTION(Client, Reliable)
	void Client_RequestPlayerInfo();

	UFUNCTION(Client, Reliable)
	void Client_RequestLobbyInfo();

	const FPlayerInfo* GetPlayerInfo() {
		return &PlayerInfo;
	}
};
