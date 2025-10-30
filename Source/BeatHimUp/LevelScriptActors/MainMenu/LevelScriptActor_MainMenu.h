// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "../../UtilityActors/PlayerPreviewer/PlayerPreviewer.h"
#include "LevelScriptActor_MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API ALevelScriptActor_MainMenu : public ALevelScriptActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "CineCameraActor")
	ACineCameraActor* CineCamera = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "PlayerPreviewerList")
	TArray<TObjectPtr<APlayerPreviewer>> PlayerPreviewerList;

	void BeginPlay() override;

	UFUNCTION(Client, Reliable)
	void SetupView();

public:
	UFUNCTION(Client, Unreliable)
	void ResetPlayerPreviewerList();
};
