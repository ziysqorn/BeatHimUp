// Fill out your copyright notice in the Description page of Project Settings.


#include "UsableItem.h"
#include "../../Interface/CanUseItem.h"

UUsableItem::UUsableItem()
{
}

void UUsableItem::OnRep_Quantity(int OldQuantity)
{
	if (ICanUseItem* CanUseItem = Cast<ICanUseItem>(this->GetOuter())) {
		if (UItemComponent* ItemComp = CanUseItem->GetItemComponent()) {
			if (ItemComp->OnItemQuantityChangedDel.IsBound()) {
				ItemComp->OnItemQuantityChangedDel.Broadcast(this);
			}
		}
	}
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
