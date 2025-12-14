// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "../../ActorComponent/PlayerHUDComponent/PlayerHUDComponent.h"
#include "MainController.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API AMainController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMainController();

	void SpectatePlayer();

	UFUNCTION(Server, Unreliable)
	void Server_SpectateNextPlayer();

	UFUNCTION(Server, Reliable)
	void Server_RequestEndGame(EMatchStatus inMatchStatus);

	UPlayerHUDComponent* GetPlayerHUDComp() {
		return PlayerHUDComp;
	}
protected:
	UPROPERTY(EditDefaultsOnly, Category = "PlayerHUDComponent")
	UPlayerHUDComponent* PlayerHUDComp = nullptr;

	UPROPERTY()
	TWeakObjectPtr<APlayerController> CurrentSpectatedPlayer = nullptr;

	int CurSpectatedPlayerIdx = -1;

	void BeginPlay() override;
	void OnPossess(APawn* aPawn) override;
	void AcknowledgePossession(APawn* aPawn) override;
};
