// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ProjectIncludes.h"
#include "MainGameState.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnBossKilled, AActor* /*Dead Actor*/, AController* /*Instigator*/, AActor* /*Damage Causer*/)
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnPlayerKilled, AActor* /*Dead Actor*/, AController* /*Instigator*/, AActor* /*Damage Causer*/)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnRepMatchStatus, EMatchStatus);

UCLASS()
class BEATHIMUP_API AMainGameState : public AGameStateBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(ReplicatedUsing = OnRep_MatchStatus, BlueprintReadOnly, Category = "MatchStatus")
	EMatchStatus MatchStatus = EMatchStatus::MATCH_BEGIN;

	UPROPERTY(ReplicatedUsing = OnRep_BossRef, BlueprintReadOnly, Category = "BossRef")
	TObjectPtr<AActor> BossRef;

	void BeginPlay() override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_BossRef(AActor* OldBossRef);

	UFUNCTION()
	void OnRep_MatchStatus(EMatchStatus OldMatchStatus);

	void SetMatchStatus(EMatchStatus inMatchStatus);

public:
	FOnRepMatchStatus OnRepMatchStatusDel;

	FOnPlayerKilled OnPlayerKilledDel;

	FOnBossKilled OnBossKilledDel;

	void SetBossRef(AActor* inBossRef) {
		BossRef = inBossRef;
	}

	void OnBossKilled(AActor* DeadActor, AController* inInstigator, AActor* Causer);

	void OnPlayerKilled(AActor* DeadActor, AController* inInstigator, AActor* Causer);

	AActor* GetBossRef() {
		return BossRef;
	}
};
