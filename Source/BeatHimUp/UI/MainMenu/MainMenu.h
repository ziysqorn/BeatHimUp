// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "../../DataAsset/UIDataAsset.h"
#include "../../MyStructs/MyStructs.h"
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

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> Txt_FriendNum;

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

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UScrollBox> ScrollBox_Friendlist;

	UPROPERTY(EditDefaultsOnly, Category = "DA_UI")
	TObjectPtr<UUIDataAsset> DA_UI;

	FTimerHandle GetFriendlistTimerHandle;


	void NativeOnInitialized() override;
	void NativeConstruct() override;
	void NativeDestruct() override;

	UFUNCTION()
	void ToggleFriendlistVisible();

	UFUNCTION()
	void ToggleLobbyAndHome();

	UFUNCTION(Client, Unreliable)
	void DisplayLogoutAlert();

	UFUNCTION(Client, Unreliable)
	void DisplayOnlyCloseAlert();

	UFUNCTION()
	void SetCustomInputMode();

	UFUNCTION()
	void ConfirmLogout();

	UFUNCTION(Client, Unreliable)
	void FriendlistMessageRecvCallback(const FString& Message);

	void InitMainMenu();
public:
	void SetUsernameText(const FText& inText) {
		if (IsValid(Txt_Username)) Txt_Username->SetText(inText);
	}

	void SetFriendNumText(int onlineNum, int totalNum) {
		FString result = TEXT("Friends ");
		result.Append(FString::FromInt(onlineNum));
		result.Append(FString("/"));
		result.Append(FString::FromInt(totalNum));
		if (IsValid(Txt_FriendNum)) Txt_FriendNum->SetText(FText::FromString(result));
	}

	void SetupFriendlist(const TArray<FPlayerInfo>& Friendlist);
};
