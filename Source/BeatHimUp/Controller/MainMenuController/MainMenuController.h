// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "../../UI/LoginScreen/LoginScreen.h"
#include "../../UI/MainMenu/MainMenu.h"
#include "../../Interface/HaveSpecialInputMode.h"
#include "MainMenuController.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API AMainMenuController : public APlayerController, public IHaveSpecialInputMode
{
	GENERATED_BODY()
	
protected:
	FHttpRequestCompleteDelegate GetFriendlistRequestCompleteDel;

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

	FTimerHandle GetFriendlistTimerHandle;

	FTimerHandle MoveToNewLevelHandle;

	float MoveToNewlevelDelay = 5.0f;

	void BeginPlay() override;

	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void SetupInputComponent() override;

	void SetupMappingContext();

	void InitForMainMenu();

	void RequestGetFriendlist();

	void OnFriendRequestReceived(const FString& Message);

	void OnFriendRequestAccepted(const FString& Message);

	void OnBeFriendRemovedReceived(const FString& Message);

	void OnLobbyInvitationReceived(const FString& Message);

	void OnDoneCreatingGameServerReceived(const FString& Message);

	void OnCreateGameServerFailedReceived(const FString& Message);

	void OnStartCreatingGameServerReceived(const FString& Message);

	void OnSentFriendRequestComplete(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully);

	void OnAcceptFriendRequestComplete(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully);

	void OnDeclineFriendRequestComplete(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully);

	void OnRemoveFriendComplete(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully);

	void OnCreateLobbyComplete(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully);

	void OnInviteToLobbyComplete(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully);

	void OnAcceptLobbyInvitationComplete(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully);

	void OnDeclineLobbyInvitationComplete(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully);

	void OnMakeLeaderComplete(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully);

	void OnLeaveLobbyComplete(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully);

	void OnKickMemberFromLobbyComplete(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully);

	void OnStartGameComplete(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully);

	void FriendlistResponseCallback(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully);

	void FriendRequestResponseCallback(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully);

	void MoveToNewLevel(FName Address);

public:

	UFUNCTION(Client, Reliable)
	void Client_CreateMainMenu();

	void BindLeftMouseRelease(UObject* userObj, FName FuncName);

	void BindLeftMouseClicked(UObject* userObj, FName FuncName);

	void SendFriendRequest(const FString& receiver);

	void AcceptFriendRequest(const FString& sender);

	void DeclineFriendRequest(const FString& sender);

	void RemoveFriend(const FString& removedFriend);

	void InviteToLobby(const FString& receiver);

	void AcceptLobbyInvitation(const FString& sender);

	void DeclineLobbyInvitation(const FString& sender);

	void MakeLeader(const FString& receiver);

	void LeaveLobby();

	void KickMemberFromLobby(const FString& receiver);

	void StartGame();

	TSubclassOf<UUserWidget> GetMenuWidgetSubclass(const FName& WidgetKey) {
		if (IsValid(MainMenu)) {
			return MainMenu->GetWidgetSubclass(WidgetKey);
		}
		return TSubclassOf<UUserWidget>();
	}

	void HandleAfterUIRemove() override;
};
