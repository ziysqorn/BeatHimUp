// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Weapon.h"
#include "Sword.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API ASword : public AWeapon
{
	GENERATED_BODY()

public:
	ASword();

protected:
	void BeginPlay() override;

	UFUNCTION()
	void ActorBeginOverlapped(AActor* OverlappedActor, AActor* OtherActor);
};
