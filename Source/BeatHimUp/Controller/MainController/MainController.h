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

	UPlayerHUDComponent* GetPlayerHUDComp() {
		return PlayerHUDComp;
	}
protected:
	UPROPERTY(EditDefaultsOnly, Category = "PlayerHUDComponent")
	UPlayerHUDComponent* PlayerHUDComp = nullptr;

	void BeginPlay() override;
	void OnPossess(APawn* aPawn) override;
	void AcknowledgePossession(APawn* aPawn) override;
};
