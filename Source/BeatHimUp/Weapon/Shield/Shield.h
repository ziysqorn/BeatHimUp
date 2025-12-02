// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Weapon.h"
#include "../../Interface/InteractableWithWeapon.h"
#include "Shield.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API AShield : public AWeapon, public IInteractableWithWeapon
{
	GENERATED_BODY()
	
public:
	AShield();

	void CancelWeaponAbility() override;

protected:
	void ResponseToAttackingWeapon(AActor* Causer) override;
};
