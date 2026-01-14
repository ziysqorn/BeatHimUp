// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "../../DataAsset/UIDataAsset.h"
#include "../../MyStructs/MyStructs.h"
#include "SideInfo.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API USideInfo : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> Txt_Username;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> Txt_FriendNum;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> WSwitcher_FriendList;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> WSwitcher_LobbyInvitation;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UButton> Btn_HideFriendList;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UButton> Btn_ShowFriendList;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UEditableTextBox> TxtBox_SearchUser;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UButton> Btn_AddFriend;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UButton> Btn_Settings;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UButton> Btn_Logout;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UScrollBox> ScrollBox_Friendlist;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> WSwitcher_FriendRequest;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UButton> Btn_PreviousRequest;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UButton> Btn_NextRequest;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UButton> Btn_PreviousInvitation;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UButton> Btn_NextInvitation;

	UPROPERTY(EditDefaultsOnly, Category = "DA_UI")
	TObjectPtr<UUIDataAsset> DA_UI;

	void NativeOnInitialized() override;
	void NativeConstruct() override;
	void NativeDestruct() override;

	UFUNCTION()
	void ToggleFriendlistVisible();

	UFUNCTION(Client, Unreliable)
	void DisplayLogoutAlert();

	UFUNCTION()
	void ConfirmLogout();

	UFUNCTION()
	void OpenSettings();

	UFUNCTION()
	void SetCustomInputMode();

	UFUNCTION()
	void NextRequest();

	UFUNCTION()
	void PrevRequest();

	UFUNCTION()
	void NextInvitation();

	UFUNCTION()
	void PrevInivtation();

	UFUNCTION()
	void SendFriendRequest();

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

	void RefreshFriendlist(int CurrentOnlineNum, const TArray<FPlayerInfo>& Friendlist);

	void FetchFriendRequest();

	void FetchLobbyInvitation();

	void RemoveReceiveFriendRequestPanel(int idx);

	void RemoveLobbyInvitationPanel(int idx);
};
