// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
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

	void BeginPlay() override;

	UFUNCTION(Client, Reliable)
	void SetupView();
};
