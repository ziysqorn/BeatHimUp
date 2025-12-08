// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Item.h"
#include "UsableItem.generated.h"

/**
 * 
 */
UCLASS()
class BEATHIMUP_API UUsableItem : public UItem
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UseItemMontageSubclassName")
	FName MontageSubclass;

	int Quantity = 1;

	int MaxQuantity = 3;

public:
	UUsableItem();

	virtual void Use() {}

	int GetQuantity() {
		return Quantity;
	}

	int GetMaxQuantity() {
		return MaxQuantity;
	}

	bool CanUseItem() const {
		return Quantity > 0;
	}

	FName GetMontageSubclassName() const {
		return MontageSubclass;
	}
};
