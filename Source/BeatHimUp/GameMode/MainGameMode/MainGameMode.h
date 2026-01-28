// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "../../UI/ChoosingTeamUI/ChoosingTeamUI.h"
#include "../../UI/PlayerTeamCard/PlayerTeamCard.h"
#include "MainGameMode.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMatchEnd, EMatchStatus);

UCLASS()
class BEATHIMUP_API AMainGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	FString Server_ID = TEXT("");

	void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	void BeginPlay() override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void PostLogin(APlayerController* NewPlayer) override;

	void Logout(AController* Exiting) override;

	void OnDropServerComplete(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully);

public:
	FOnMatchEnd OnMatchEndDel;

	void OnBossKilled();

	void OnPlayerKilled();

	void EndMatch(EMatchStatus inMatchStatus);

	void StartEndMatch(EMatchStatus inMatchStatus);

	void SetServerID(const FString& newID) {
		Server_ID = newID;
	}
};
