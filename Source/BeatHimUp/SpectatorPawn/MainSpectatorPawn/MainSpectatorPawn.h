// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "MainSpectatorPawn.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API AMainSpectatorPawn : public ASpectatorPawn
{
	GENERATED_BODY()

public:
	AMainSpectatorPawn();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "EditorProperties|Input")
	UInputMappingContext* MC_SpectatorMode = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "EditorProperties|Input|Input Action");
	UInputAction* IA_NextSpectatedPlayer = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "EditorProperties|Input|Input Action");
	UInputAction* IA_PauseGame = nullptr;

	void BeginPlay() override;

	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ViewNextPlayer();

	void SetupMappingContext();
};
