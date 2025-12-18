// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "MainMenuGameMode.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FPostLogin, APlayerController*)

UCLASS()
class BEATHIMUP_API AMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	FPostLogin PostLoginDel;

protected:
	void BeginPlay() override;
	void PostLogin(APlayerController* NewPlayer) override;
};
