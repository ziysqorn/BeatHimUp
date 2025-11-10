// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "HitStopComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BEATHIMUP_API UHitStopComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHitStopComponent();

protected:
	UPROPERTY()
	FTimerHandle HitStopTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "CameraShakeSubclass")
	TSubclassOf<UCameraShakeBase> CameraShakeSubclass;

	void NetMulticast_EndHitStop();

	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UFUNCTION(NetMulticast, Unreliable)
	void NetMulticast_HitStop(float HitStopDuration, float HitStopDilation);

	UFUNCTION(Client, Unreliable)
	void Client_ShakeCameraOnHit();
};
