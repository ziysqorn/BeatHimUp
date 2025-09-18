// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ProjectIncludes.h"
#include "../CustomInfo/TeamState/TeamState.h"
#include "MainPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API AMainPlayerState : public APlayerState
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(Replicated)
	TObjectPtr<ATeamState> OwnedTeam;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	AMainPlayerState();

	void SetOwningTeam(const TObjectPtr<ATeamState>& InTeam) {
		OwnedTeam = InTeam;
	}
};
