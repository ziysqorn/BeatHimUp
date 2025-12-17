// Fill out your copyright notice in the Description page of Project Settings.


#include "UsableItem.h"

UUsableItem::UUsableItem()
{
}

void UUsableItem::OnRep_Quantity(int OldQuantity)
{
}

void UUsableItem::OnRep_MaxQuantity(int OldMaxQuantity)
{
}

void UUsableItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UUsableItem, Quantity);

	DOREPLIFETIME(UUsableItem, MaxQuantity);
}
