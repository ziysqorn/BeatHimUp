// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "TeamState.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API ATeamState : public AInfo
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(Replicated)
	FName TeamName;

	UPROPERTY(Replicated)
	TArray<APlayerState*> Members;

	UPROPERTY(Replicated)
	uint8 TeamCapacity;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	ATeamState();
	//Return false if team has reached maximum capacity
	bool AddMember(APlayerState* InMember) {
		if (Members.Num() == TeamCapacity) return false;
		Members.Add(InMember);
		return true;
	}

};
