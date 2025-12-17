// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPotion.h"
#include "../../Interface/CanUseItem.h"

UHealthPotion::UHealthPotion()
{
	Quantity = 3;
}

void UHealthPotion::Use_Implementation()
{
	if (Quantity <= 0) return;
	if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(this->GetOuter())) {
		if (UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent()) {
			FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
			ContextHandle.AddInstigator(Cast<AActor>(this->GetOuter()), Cast<AActor>(this->GetOuter()));
			ContextHandle.AddSourceObject(this);
			
			FGameplayEffectSpecHandle GESpecHandle = ASC->MakeOutgoingSpec(GESubclass_IncreaseHealth, Level, ContextHandle);

			if(GESpecHandle.IsValid())
				ASC->ApplyGameplayEffectSpecToSelf(*GESpecHandle.Data.Get());
		}
	}
	--Quantity;
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Format(TEXT("HEALTH POTION USED ! - CurrentQuantity: {0}"), {Quantity}));
}

void UHealthPotion::OnRep_Quantity(int OldQuantity) 
{
	if (ICanUseItem* CanUseItem = Cast<ICanUseItem>(this->GetOuter())) {
		if (UItemComponent* ItemComp = CanUseItem->GetItemComponent()) {
			if (ItemComp->OnItemQuantityChangedDel.IsBound()) {
				ItemComp->OnItemQuantityChangedDel.Broadcast(this);
			}
		}
	}
}

void UHealthPotion::OnRep_MaxQuantity(int OldMaxQuantity) 
{

}
