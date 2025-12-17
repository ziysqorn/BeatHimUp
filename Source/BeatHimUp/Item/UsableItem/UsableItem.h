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

	UPROPERTY(Replicated, ReplicatedUsing = "OnRep_Quantity", EditDefaultsOnly, Category = "EditorProperties | Quantity")
	int Quantity = 1;

	UPROPERTY(Replicated, ReplicatedUsing = "OnRep_MaxQuantity", EditDefaultsOnly, Category = "EditorProperties | MaxQuantity")
	int MaxQuantity = 3;

	UFUNCTION()
	virtual void OnRep_Quantity(int OldQuantity);

	UFUNCTION()
	virtual void OnRep_MaxQuantity(int OldMaxQuantity);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

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
