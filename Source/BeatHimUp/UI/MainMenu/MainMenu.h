// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "../OnScreenAlert/OnScreenAlert.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UTextBlock> Txt_Username;

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UWidgetSwitcher> WSwitcher_FriendList;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> WSwitcher_MainScreen;

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UButton> Btn_HideFriendList;

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UButton> Btn_ShowFriendList;

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UButton> Btn_ToLobby;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UButton> Btn_Home;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UButton> Btn_Play;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UButton> Btn_Logout;

	UPROPERTY(EditDefaultsOnly, Category = "ScreenAlertSubclass")
	TSubclassOf<UOnScreenAlert> ScreenAlertSubclass;


	void NativeOnInitialized() override;
	void NativeConstruct() override;
	void NativeDestruct() override;

	UFUNCTION()
	void ToggleFriendlistVisible();

	UFUNCTION()
	void ToggleLobbyAndHome();

	UFUNCTION(Client, Unreliable)
	void DisplayLogoutAlert();

	UFUNCTION()
	void SetCustomInputMode();

	UFUNCTION()
	void ConfirmLogout();

	void InitMainMenu();
public:
	void SetUsernameText(const FText& inText) {
		if (IsValid(Txt_Username)) Txt_Username->SetText(inText);
	}
};
