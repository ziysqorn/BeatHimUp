// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Weapon.h"
#include "MonsterWiping.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API AMonsterWiping : public AWeapon
{
	GENERATED_BODY()

public:
	AMonsterWiping();

protected:
	void BeginPlay() override;

	UFUNCTION()
	void BoxCompBeginOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void BoxCompEndOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
