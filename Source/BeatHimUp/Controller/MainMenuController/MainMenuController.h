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
	UPROPERTY(EditDefaultsOnly, Category = "MainMenuSubclass")
	TSubclassOf<UMainMenu> MainMenuSubclass;

	UPROPERTY()
	TObjectPtr<UMainMenu> MainMenu;

	UPROPERTY(EditDefaultsOnly, Category = "InputMappingContext")
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "IA_LeftMouse")
	TObjectPtr<UInputAction> IA_LeftMouse;

	UPROPERTY(EditDefaultsOnly, Category = "IA_LeftMouseClicked")
	TObjectPtr<UInputAction> IA_LeftMouseClicked;

	void BeginPlay() override;

	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void SetupInputComponent() override;

	void SetupMappingContext();

	void LogoutRequestComplete(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully);

public:

	UFUNCTION(Client, Reliable)
	void Client_CreateMainMenu();

	void BindLeftMouseRelease(UObject* userObj, FName FuncName);

	void BindLeftMouseClicked(UObject* userObj, FName FuncName);
};
