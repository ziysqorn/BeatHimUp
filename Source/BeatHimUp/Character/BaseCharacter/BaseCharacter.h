// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ProjectIncludes.h"
#include "../../ActorComponent/WeaponComponent/WeaponComponent.h"
#include "../../ActorComponent/HitStopComponent/HitStopComponent.h"
#include "BaseCharacter.generated.h"

UCLASS()
class BEATHIMUP_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components | WeaponComponent")
	UWeaponComponent* WeaponComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components | HitStopComponent")
	UHitStopComponent* HitStopComp = nullptr;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	UWeaponComponent* GetWeaponComponent() {
		return WeaponComponent;
	}
};
