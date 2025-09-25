// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "../../UI/ChoosingTeamUI/ChoosingTeamUI.h"
#include "../../UI/PlayerTeamCard/PlayerTeamCard.h"
#include "../../CustomInfo/TeamState/TeamState.h"
#include "MainGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API AMainGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(Replicated)
	TArray<ATeamState*> Teams;

	void BeginPlay() override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void PostLogin(APlayerController* NewPlayer) override;

};
