// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UsableItem.h"
#include "HealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API UHealthPotion : public UUsableItem
{
	GENERATED_BODY()

public:
	UHealthPotion();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "HealthPotionLevel")
	float Level = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "IncreaseHealthGESubclass")
	TSubclassOf<UGameplayEffect> GESubclass_IncreaseHealth;

	UFUNCTION(Server, Reliable)
	void Use() override; 

	void OnRep_Quantity(int OldQuantity) override;

	void OnRep_MaxQuantity(int OldMaxQuantity) override;
};
