// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemComponent.h"


// Sets default values for this component's properties
UItemComponent::UItemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	//
	bReplicateUsingRegisteredSubObjectList = true;

	SetIsReplicatedByDefault(true);
}

void UItemComponent::UseItem()
{
	if (UsableItemList.IsValidIndex(CurrentItemIdx) && IsValid(UsableItemList[CurrentItemIdx])) {
		UsableItemList[CurrentItemIdx]->Use();
	}
}

void UItemComponent::SwitchUsableItem()
{
	if (!UsableItemList.IsValidIndex(CurrentItemIdx)) {
		CurrentItemIdx = 0;
		return;
	}
	++CurrentItemIdx;
}

void UItemComponent::AddUsableItemToList(UUsableItem* Item)
{
	this->AddReplicatedSubObject(Item);
	UsableItemList.Add(Item);
}

void UItemComponent::RemoveUsableItemFromList(UUsableItem* Item)
{
	if (UsableItemList.Find(Item) != INDEX_NONE) {
		this->RemoveReplicatedSubObject(Item);
		UsableItemList.Remove(Item);
	}
}


// Called when the game starts
void UItemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void UItemComponent::Server_InitUsableItemList_Implementation()
{
	if (IsValid(DT_Items)) {
		FItemData* HealthPotionData = DT_Items->FindRow<FItemData>(FName("HealthPotion"), TEXT("Lookup"));
		if (HealthPotionData) {
			AddUsableItemToList(NewObject<UUsableItem>(this->GetOwner(), HealthPotionData->ItemSubclass));
		}
	}
	CurrentItemIdx = 0;
}

void UItemComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UItemComponent, UsableItemList);
}

void UItemComponent::OnRep_UsableItemList(const TArray<UUsableItem*>& OldUsableItemList)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, TEXT("Item list replicated !"));
	if (OnRepUsableItemListDel.IsBound()) {
		OnRepUsableItemListDel.Broadcast(UsableItemList);
	}
}

