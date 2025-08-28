// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ProjectIncludes.h"
#include "../Weapon/Weapon.h"
#include "WeaponDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API UWeaponDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Subclass Map")
	TMap<FName, TSubclassOf<AWeapon>> WeaponSubclassMap;
};
