// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "../../Interface/HaveSpecialInputMode.h"
#include "../../ActorComponent/PlayerHUDComponent/PlayerHUDComponent.h"
#include "MainController.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API AMainController : public APlayerController, public IHaveSpecialInputMode
{
	GENERATED_BODY()
	
public:
	AMainController();

	void SwitchToSpectate();

	UFUNCTION(Server, Reliable)
	void Server_RequestEndGame(EMatchStatus inMatchStatus);

	UFUNCTION(Client, Reliable)
	void Client_TravelToMap(FName MapName);

	void EndGameProcess(EMatchStatus inMatchStatus);

	void SetWidgetToLockTarget(AActor* Target);

	void PauseGame();

	UPlayerHUDComponent* GetPlayerHUDComp() {
		return PlayerHUDComp;
	}

	void HandleAfterUIRemove() override;

	void SaveCharacterStats(TSharedPtr<FJsonObject> JsonObj);
protected:
	UPROPERTY()
	TObjectPtr<AActor> TargetLockPointWidgetActor = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "PlayerHUDComponent")
	UPlayerHUDComponent* PlayerHUDComp = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "TargetLockPointWidgetActorSubclass")
	TSubclassOf<AActor> TargetLockPointWidgetActorSubclass;

	UPROPERTY()
	TWeakObjectPtr<APlayerController> CurrentSpectatedPlayer = nullptr;

	int CurSpectatedPlayerIdx = -1;

	void BeginPlay() override;
	void OnPossess(APawn* aPawn) override;
	void AcknowledgePossession(APawn* aPawn) override;

	UFUNCTION(Server, Reliable)
	void Server_FetchCharacterStats(const FString& JsonStr);

	void UnbindControlledPawnHUD();

	UFUNCTION(Client, Reliable)
	void Client_UpdateNewSpectatedPlayerHUD(AActor* SpectatedActor);

	UFUNCTION(Client, Reliable)
	void Client_UnbindPreviousSpectatedPlayerHUD(AActor* PreviousSpectactedPawn);

	UFUNCTION(Client, Reliable)
	void Client_UpdateHUDItemQuantity(UUsableItem* Item, AActor* CurrentViewTargetPawn);

	UFUNCTION(Server, Reliable)
	void Server_RequestViewTargetPawnToUpdateItemFrameQuantity(UUsableItem* Item);

	void OnGetCharacterStatsComplete(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully);

	void SetSpectatorPawn(ASpectatorPawn* NewSpectatorPawn) override;

	APlayerState* GetNextViewablePlayer(int32 dir) override;

	void SetViewTarget(class AActor* NewViewTarget, FViewTargetTransitionParams TransitionParams = FViewTargetTransitionParams()) override;
};
