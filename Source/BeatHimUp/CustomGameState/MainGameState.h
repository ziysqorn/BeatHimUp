// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ProjectIncludes.h"
#include "MainGameState.generated.h"

/**
 * 
 */

UCLASS()
class BEATHIMUP_API AMainGameState : public AGameStateBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(ReplicatedUsing = OnRep_BossRef, BlueprintReadOnly, Category = "BossRef")
	TObjectPtr<AActor> BossRef;

	void BeginPlay() override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_BossRef(AActor* OldBossRef);

public:
	void SetBossRef(AActor* inBossRef) {
		BossRef = inBossRef;
	}

	AActor* GetBossRef() {
		return BossRef;
	}
};
