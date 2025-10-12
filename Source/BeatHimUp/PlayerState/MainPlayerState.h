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
	UPROPERTY(Replicated, ReplicatedUsing=OnRep_Username)
	FName Username;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	virtual void OnRep_Username() {
		FString _Role = HasAuthority() ? TEXT("Server") : TEXT("Client");
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("[%s] Username replicated: %s"), *_Role, *Username.ToString()));
	}

public:
	AMainPlayerState();

	const FName& GetUsername() {
		return Username;
	}

	UFUNCTION(Server, Reliable)
	void SetUsername(const FName& inName);
};
