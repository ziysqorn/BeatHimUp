// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "../../DataAsset/UIDataAsset.h"
#include "../../MyStructs/MyStructs.h"
#include "../SideInfo/SideInfo.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> WSwitcher_MainScreen;

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UButton> Btn_ToLobby;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UButton> Btn_Home;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UButton> Btn_Play;

	UPROPERTY(EditDefaultsOnly, Category = "DA_UI")
	TObjectPtr<UUIDataAsset> DA_UI;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UNamedSlot> NamedSlot_SideInfo;

	UPROPERTY(EditDefaultsOnly, Category = "SideInfoSubclass")
	TSubclassOf<USideInfo> SideInfoSubclass;

	void NativeOnInitialized() override;
	void NativeConstruct() override;
	void NativeDestruct() override;

	UFUNCTION()
	void ToggleLobbyAndHome();

	void InitMainMenu();

	FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
public:
	UFUNCTION(Client, Unreliable)
	void DisplayOnlyCloseAlert(const FString& message);

	void SetUsernameText(const FText& inText) {
		if (IsValid(NamedSlot_SideInfo)) {
			if (USideInfo* SideInfo = Cast<USideInfo>(NamedSlot_SideInfo->GetChildAt(0))) {
				SideInfo->SetUsernameText(inText);
			}
		}
	}

	void RefreshFriendlist(int CurrentOnlineNum, const TArray<FPlayerInfo>& Friendlist) {
		if (IsValid(NamedSlot_SideInfo)) {
			if (USideInfo* SideInfo = Cast<USideInfo>(NamedSlot_SideInfo->GetChildAt(0))) {
				SideInfo->RefreshFriendlist(CurrentOnlineNum, Friendlist);
			}
		}
	}

	void FetchFriendRequest() {
		if (IsValid(NamedSlot_SideInfo)) {
			if (USideInfo* SideInfo = Cast<USideInfo>(NamedSlot_SideInfo->GetChildAt(0))) {
				SideInfo->FetchFriendRequest();
			}
		}
	}

	void FetchLobbyInvitation() {
		if (IsValid(NamedSlot_SideInfo)) {
			if (USideInfo* SideInfo = Cast<USideInfo>(NamedSlot_SideInfo->GetChildAt(0))) {
				SideInfo->FetchLobbyInvitation();
			}
		}
	}

	void RemoveReceiveFriendRequestPanel(int idx) {
		if (IsValid(NamedSlot_SideInfo)) {
			if (USideInfo* SideInfo = Cast<USideInfo>(NamedSlot_SideInfo->GetChildAt(0))) {
				SideInfo->RemoveReceiveFriendRequestPanel(idx);
			}
		}
	}

	void RemoveLobbyInvitationPanel(int idx) {
		if (IsValid(NamedSlot_SideInfo)) {
			if (USideInfo* SideInfo = Cast<USideInfo>(NamedSlot_SideInfo->GetChildAt(0))) {
				SideInfo->RemoveLobbyInvitationPanel(idx);
			}
		}
	}
};
