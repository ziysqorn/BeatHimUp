// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "../../UI/LoginScreen/LoginScreen.h"
#include "../../UI/MainMenu/MainMenu.h"
#include "MainMenuController.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API AMainMenuController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "LoginScreenSubclass")
	TSubclassOf<ULoginScreen> LoginScreenSubclass;

	UPROPERTY(EditDefaultsOnly, Category = "MainMenuSubclass")
	TSubclassOf<UMainMenu> MainMenuSubclass;

	UPROPERTY()
	TObjectPtr<ULoginScreen> LoginScreen;

	UPROPERTY()
	TObjectPtr<UMainMenu> MainMenu;

	void BeginPlay() override;

	UFUNCTION(Client, Reliable)
	void Client_DisplayLoginScreen();

public:
	UFUNCTION(Client, Reliable)
	void Client_DisplayMainMenu();
};
