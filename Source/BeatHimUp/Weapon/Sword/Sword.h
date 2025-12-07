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
	void BoxCompBeginOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void BoxCompEndOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
