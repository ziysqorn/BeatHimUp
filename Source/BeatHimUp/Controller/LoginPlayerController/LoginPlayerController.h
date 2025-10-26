// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "../../UI/LoginScreen/LoginScreen.h"
#include "LoginPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API ALoginPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "LoginScreenSubclass")
	TSubclassOf<ULoginScreen> LoginScreenSubclass;

	UPROPERTY()
	TObjectPtr<ULoginScreen> LoginScreen;

	void BeginPlay() override;

public:
	UFUNCTION(Client, Reliable)
	void Client_DisplayLoginScreen();
};
