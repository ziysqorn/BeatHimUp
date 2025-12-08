// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPotion.h"

UHealthPotion::UHealthPotion()
{
	Quantity = 3;
}

void UHealthPotion::Use_Implementation()
{
	if (Quantity <= 0) return;
	--Quantity;
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Format(TEXT("HEALTH POTION USED ! - CurrentQuantity: {0}"), {Quantity}));
}
